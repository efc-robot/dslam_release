#!/usr/bin/env python
import torch
import torch.nn as nn
import torchvision
import torch.nn.functional as F
import torchvision.transforms as transforms
import torchvision.models as models
import time
import numpy as np
from PIL import Image
import os

def main():
    
    # load model
    net = ImageRetrievalNet()
    state = torch.load('/home/xuzhl/catkin_ws_d/src/dslam_release/script/ROS_GEM/weights/gem.pth')
    net.load_state_dict(state['state_dict'], strict=False)
    
    # image preprocess
    normalize = transforms.Normalize(
        mean=[0.485, 0.456, 0.406],
        std=[0.229, 0.224, 0.225]
    )
    transform = transforms.Compose([
        transforms.Resize(256),
        transforms.ToTensor(),
        normalize
    ])
    
    # list of image names
    images = ['/home/xuzhl/dataset/image_503_loop/pic/left_7.png']
    qimages = ['/home/xuzhl/dataset/image_503_loop/pic/left_2665.png']
    
    # extract features
    while(True):
        vecs = extract_vectors(net, images, transform)
        qvecs = extract_vectors(net, qimages, transform)
        
        # match result
        best_match = match(vecs, qvecs)
        time.sleep(0.1)
    print(best_match)
    
def match(vecs, qvecs, threshold=0.95):
    # return matching image index if succeed;
    # return -1 if fail
    scores = np.dot(vecs.T, qvecs)
    ranks = np.argsort(-scores, axis=0)
    max_scores = np.max(scores, axis=0)
    best_match = ranks[0,:]
    best_match[max_scores < threshold] = -1
    return best_match


def extract_vectors(net, images, transform):
    net.cuda()
    vecs = np.zeros((net.meta['outputdim'], len(images)))
    for i in range(len(images)):
        fn = images[i]
        with open(fn, 'rb') as f:
            img = Image.open(f)
            img = img.convert('RGB')
        v = net(transform(img).unsqueeze(0).cuda())
        vecs[:,i] = v.cpu().data.squeeze()
        if (i+1) % 10 == 0 or (i+1) == len(images):
            print('\r>>>> {}/{} done...'.format((i+1), len(images)))
    return vecs

class GeM(nn.Module):
    def __init__(self, eps=1e-6):
        super(GeM, self).__init__()
        self.eps = eps

    def forward(self, x):
        return F.avg_pool2d(x.clamp(min=self.eps).pow(3), (x.size(-2), x.size(-1))).pow(1./3)
    
class L2N(nn.Module):
    def __init__(self, eps=1e-6):
        super(L2N,self).__init__()
        self.eps = eps

    def forward(self, x):
        return x / (torch.norm(x, p=2, dim=1, keepdim=True) + self.eps).expand_as(x)
    
class ImageRetrievalNet(nn.Module):
    def __init__(self):
        super(ImageRetrievalNet, self).__init__()
        net_in = getattr(torchvision.models, 'resnet101')(pretrained=False)
        features = list(net_in.children())[:-2]
        self.features = nn.Sequential(*features)
        self.pool = GeM()
        self.norm = L2N()
        meta = {'outputdim': 2048}
        self.meta = meta
    def forward(self, x):
        # x -> features
        o = self.features(x)
        
        # features -> pool -> norm
        o = self.norm(self.pool(o)).squeeze(-1).squeeze(-1)
        # permute so that it is Dx1 column vector per image (DxN if many images)
        return o.permute(1,0)
        
if __name__ == '__main__':
    main()