# ROS-based DSLAM
该项目的目的是构建完全给予ROS的模块化 DLSAM 系统, 现在可以执行多机器人轨迹合并和优化。

## 准备工作
- 项目基于ubuntu，需要安装 ROS
- 现在的项目移除了测试superpoint

## 依赖环境
- opencv （讲道理在 ubuntu18.04 中，直接 sudo apt install libopencv-dev 即可。注意先安装 ROS 再安装 opencv
- 需要编译安装 g2o 仓库
    - 【重要】在安装 g2o 之前，需要先 sudo apt install libsuitesparse-dev。不然编译的库没法用
    - g2o 仓库地址 https://github.com/RainerKuemmerle/g2o.git
- 编译安装图优化库(可以安装4.0.3 tag版本）
    - https://github.com/borglab/gtsam.git
    (git clone --branch 4.0.3 https://github.com/borglab/gtsam.git)
    - 我们只在python中用到图优化，需要编译python wrap， 教程参考 https://github.com/borglab/gtsam/cython/README.md
- python2.7
    - pip install torch
    - pip install future
    - pip install torchvision
- OpenCV 3.2.0 + OpenCV Contrib 3.2.0 （高于opencv3.4.3.16之后由于版权原因SIFT无法使用，且会报错cv::Exception）
- Eigen3

## 修改内容
- 修改CMakeLists.txt中的第9行：G2O_CMAKE_MODULES的路径为本机中<path_to_g2o>/g2o/cmake_modules/。
- 修改所有的launch file中gemweightspath (gem.pth) 为本机中<path_to_DSLAM_one>/DSLAM_one/src/ROS-DSLAM/script/ROS_GEM/weights/gem.pth
- 修改所有的launch file中datasetpath路径为本机<path_to_image_503_loop>/image_503_loop/pic
- ros内 OpenCV 3.2.0如果通过编译安装，需要额外将安装好的头文件/usr/local/include/opencv*复制到/usr/include/下
## 编译使用

- 切换到工作空间，并新建src文件家
    ```Shell
    cd ~/catkin_ws_d
    mkdir src
    cd  src
    ```

- 下载代码，同时下载octomap_server包
    ```Shell
    git clone https://github.com/efc-robot/dslam_release.git
    git clone https://github.com/xxzzll11111/octomap_server.git
    cd dslam_release
    ```

- 使用catkin_make 编译
    ```Shell
    cd ~/catkin_ws_d
    catkin_make
    ```

## 在ROS中运行
要先准备数据集，不然会报错，数据集可以问我要。

默认数据集位置是 "/home/yujc/robotws/dataset/image_503_loop/"


```
source ~/catkin_ws_d/devel/setup.bash
roslaunch dslam_sp SP_VO_file.launch
```

但是SP可能需要显卡资源比较多。
可以只跑orb的方法。然后在rviz中可视化。
```
source ~/catkin_ws_d/devel/setup.bash
roslaunch dslam_sp ORB_VO_file_loop.launch
```

注意修改 launch file 中的路径。

## 待调bug（feature work）
1. rgbd2pointcloud节点可以放在前面，merge_img_depth_node之后，中间的数据通路可以删除掉image信息
2. FeatureExtract节点的输出descriptor可以直接用sensor_msgs/Image的数据类型来传输
3. VO_loop节点内部的descriptor不需要从mat转成msg再转回mat
