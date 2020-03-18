float32     angle       #computed orientation of the keypoint (-1 if not
                        # applicable); it's in [0,360) degrees and measured
                        #relative to image coordinate system, ie in clockwise.

int32       class_id 	#object class (if the keypoints need to be clustered
                        # by an object they belong to)

int32 	    octave      #octave (pyramid layer) from which the keypoint has
                        # been extracted

Point2f     pt          #coordinates of the keypoints

float32 	response    #the response by which the most strong keypoints
                        # have been selected. Can be used for the further
                        #sorting or subsampling                    
                        
float32 	size 	    #diameter of the meaningful keypoint 
                        #neighborhood                        