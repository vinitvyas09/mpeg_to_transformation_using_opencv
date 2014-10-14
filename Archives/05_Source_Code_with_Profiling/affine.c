#include "affine.h"

int number_of_files();

int main( int argc, char **argv)
{


	/*----------------------- Affine : Declaring the variables ----------------------*/
	
	unsigned int i=0;
	int j=0;
	int max_files;
	int once[4]={0};
	int option;
	

	/*-----------------------Time Profiling Structs-------------------------*/

	struct timeval sobel_start, sobel_end;
	struct timeval affine_start, affine_end;
	struct timeval total_start, total_end;

	unsigned int sobel_min, sobel_sec, sobel_msec;
	unsigned int affine_min, affine_sec, affine_msec;
	unsigned int total_min, total_sec, total_msec;
	
	int section=0;
	int block_size;

	char frame_name[19];
	char frame_name2[25];
	char frame_name3[26];
	
	Point2f srcTri[4];
	Point2f dstTri[4];

	Mat rot_mat( 2, 3, CV_32FC1 );
	Mat warp_mat( 2, 3, CV_32FC1 );

	// Output variables
	Mat src, warp_dst, warp_rotate_dst;

	
	// Going to 'root' from 'Source_Code'

	if(chdir("..")!=EXIT_SUCCESS)
	{
		perror("Executing command 'cd ..'");
	}

	// Compute the number of files in the folder
	
	//max_files=number_of_files();
	// Please check the function number_of_files() 
	// for explanation

	max_files=3601;	//Total number of Frames

	if(chdir("./Original_Frames/")!=EXIT_SUCCESS)
	{
		perror("Accessing Directory 'Original_Frames'");
	}


	block_size=max_files/4;
	float pos[8]={0,0,1,0,0,1,1,1};
	do
	{
		printf("Enter the option:\n");
		printf("1-> Both Sobel and Affine\n");
		printf("2-> Only Sobel\n");
		printf("3-> Only Affine\n");

		if( scanf("%d",&option) != 1)
		{
			perror("Unexpected input from user");
		}
	}
	while(!((option==1)||(option==2)||(option==3)));

	gettimeofday(&total_start,NULL);
	gettimeofday(&sobel_start,NULL);

	if (option!=3)
	{


		/*----------------------- Sobel : Declaring the variables ----------------------*/


		Mat sobel_src, src_gray, s_src;
		Mat grad;

		int c;

		struct stat st = {0};

		printf("Starting Sobel Filtering!!\n"); 
		printf("Applying Sobel Filtering...\n"); 


		/*------------------------- Starting the loop --------------------------*/

		for (i=1; i<=max_files; i++)
		{

			/*------------------------- Loading the Image --------------------------*/


			// Select the right frame

			sprintf(frame_name,"Frame_no_%05u.ppm",i);

			// Load the Image
	
			sobel_src = imread( frame_name,1 );  

			if( !sobel_src.data )
			{
				perror("Reading from file"); 
			}


			// Apply Gaussian Blur function

			GaussianBlur( sobel_src, s_src, Size(3,3), 0, 0, BORDER_DEFAULT );

			// Convert Color image to gray
			//printf("cvtcolor\n"); 

			cvtColor( s_src, src_gray, CV_RGB2GRAY );
			//printf("after cvtcolor\n"); 
			// Generate x and y matrix

			Mat sobel_x, sobel_y;
			Mat sobel_abs_x, sobel_abs_y;

			// Gradient X

			Sobel( src_gray, sobel_x, SOBEL_DEPTH, 1, 0, 3, SOBEL_SCALE, SOBEL_DELTA, BORDER_DEFAULT );
			convertScaleAbs( sobel_x, sobel_abs_x );

			// Gradient Y

			Sobel( src_gray, sobel_y, SOBEL_DEPTH, 0, 1, 3, SOBEL_SCALE, SOBEL_DELTA, BORDER_DEFAULT );
			convertScaleAbs( sobel_y, sobel_abs_y );

			// Total Gradient
			addWeighted( sobel_abs_x, 0.5, sobel_abs_y, 0.5, 0, grad );

			/*------------------------- Storing the Image ---------------------------*/

			// Storing the Image

			sprintf(frame_name2,"Sobel_frame_no_%05u.ppm",i);
	
			imwrite(frame_name2, grad);

		}
		// End of 'for' loop
		printf("Sobel Filtering done!!\n"); 
	}
	// end 'if (option!=3)'
	
	gettimeofday(&sobel_end,NULL);
	sobel_sec=sobel_end.tv_sec-sobel_start.tv_sec;
	sobel_min=sobel_sec/60;
	sobel_sec=sobel_sec%60;
	sobel_msec=sobel_end.tv_usec-sobel_start.tv_usec;
	sobel_msec/=1000;
	//printf("Sobel Transform Timings  = %d minutes:%d seconds\n",sobel_min,sobel_sec);

	printf("Sleeping for 5 seconds\n");
	sleep(5);

	/******************************************AFFINE************************************************/

	// Time profiling
	gettimeofday(&affine_start,NULL);

	if (option!=2)
	{

		printf("Starting Affine Filtering!!\n"); 
		printf("Applying Affine Filtering...\n"); 


		/*------------------------- Starting the loop --------------------------*/

		for (i=1; i<=max_files; i++)
		{

			/*------------------------- Loading the Image --------------------------*/

			if(option==1)
			{
				// Select the right frame
				sprintf(frame_name2,"Sobel_frame_no_%05u.ppm",i);
				// Load the Image
				src = imread( frame_name2, 1 );
			}

			else
			{
				sprintf(frame_name,"Frame_no_%05u.ppm",i);
				src = imread( frame_name, 1 );
			}

			/*---------------------- Affine Transform : Warp -----------------------*/

			// Setting up the output image parameters

			warp_dst = Mat::zeros( src.rows, src.cols, src.type() );


			/*---------------------- Change the parameter values ----------------------*/

		
		
			switch(section)
			{

				case 0:
				{

					pos[1]=pos[1]+0.001;
					pos[2]=pos[2]-0.001;
					pos[4]=pos[4]+0.001;
					pos[7]=pos[7]-0.001;
			
				
					// Setting parameters for matrix computation
	
					srcTri[0] = Point2f( 0,0 );
					srcTri[1] = Point2f( src.cols - 1, 0 );
					srcTri[2] = Point2f( 0, src.rows - 1 );
					srcTri[3] = Point2f( src.cols - 1, src.rows - 1 );

					dstTri[0] = Point2f( src.cols*pos[0], src.rows*pos[1] );
					dstTri[1] = Point2f( src.cols*pos[2], src.rows*pos[3] );
					dstTri[2] = Point2f( src.cols*pos[4], src.rows*pos[5] );
					dstTri[3] = Point2f( src.cols*pos[6], src.rows*pos[7] );
				
					section=i/block_size;

					//printf("Case 0: %u\t %f %f %f %f %f %f %f %f\n",i,pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);

					break;
				}

				case 1:
				{

					pos[0]=pos[0]+0.001;
					pos[3]=pos[3]+0.001;
					pos[5]=pos[5]-0.001;
					pos[6]=pos[6]-0.001;
			
				
					// Setting parameters for matrix computation
	
					srcTri[0] = Point2f( 0,0 );
					srcTri[1] = Point2f( src.cols - 1, 0 );
					srcTri[2] = Point2f( 0, src.rows - 1 );
					srcTri[3] = Point2f( src.cols - 1, src.rows - 1 );

					dstTri[0] = Point2f( src.cols*pos[0], src.rows*pos[1] );
					dstTri[1] = Point2f( src.cols*pos[2], src.rows*pos[3] );
					dstTri[2] = Point2f( src.cols*pos[4], src.rows*pos[5] );
					dstTri[3] = Point2f( src.cols*pos[6], src.rows*pos[7] );
				
					section=i/block_size;

					//printf("Case 1: %u\t %f %f %f %f %f %f %f %f\n",i,pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);

					break;
				}
			
				case 2:
				{
				
					pos[1]=pos[1]-0.001;
					pos[2]=pos[2]+0.001;
					pos[4]=pos[4]-0.001;
					pos[7]=pos[7]+0.001;
			
				
					// Setting parameters for matrix computation
	
					srcTri[0] = Point2f( 0,0 );
					srcTri[1] = Point2f( src.cols - 1, 0 );
					srcTri[2] = Point2f( 0, src.rows - 1 );
					srcTri[3] = Point2f( src.cols - 1, src.rows - 1 );

					dstTri[0] = Point2f( src.cols*pos[0], src.rows*pos[1] );
					dstTri[1] = Point2f( src.cols*pos[2], src.rows*pos[3] );
					dstTri[2] = Point2f( src.cols*pos[4], src.rows*pos[5] );
					dstTri[3] = Point2f( src.cols*pos[6], src.rows*pos[7] );
				
					section=i/block_size;

					//printf("Case 2: %u\t %f %f %f %f %f %f %f %f\n",i,pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);

					break;
				}
			

				case 3:
				{

					pos[0]=pos[0]-0.001;
					pos[3]=pos[3]-0.001;
					pos[5]=pos[5]+0.001;
					pos[6]=pos[6]+0.001;
			
				
					// Setting parameters for matrix computation
	
					srcTri[0] = Point2f( 0,0 );
					srcTri[1] = Point2f( src.cols - 1, 0 );
					srcTri[2] = Point2f( 0, src.rows - 1 );
					srcTri[3] = Point2f( src.cols - 1, src.rows - 1 );

					dstTri[0] = Point2f( src.cols*pos[0], src.rows*pos[1] );
					dstTri[1] = Point2f( src.cols*pos[2], src.rows*pos[3] );
					dstTri[2] = Point2f( src.cols*pos[4], src.rows*pos[5] );
					dstTri[3] = Point2f( src.cols*pos[6], src.rows*pos[7] );

				
					section=i/block_size;

					//printf("Case 3: %u\t %f %f %f %f %f %f %f %f\n",i,pos[0],pos[1],pos[2],pos[3],pos[4],pos[5],pos[6],pos[7]);

					break;
				}

				default:
				{
					//printf("Value: %d\n",section);
					//perror("Default switch() case");
					break;
				}
			}
			



			// Calculate the Affine Transform matrix

			warp_mat = getAffineTransform( srcTri, dstTri );


			// Applying the Affine Transform to the src image

			warpAffine( src, warp_dst, warp_mat, warp_dst.size() );



			/*-------------------- Affine Transform : Rotate -----------------------*/

			// Compute the Rotation Matrix Parameters

			Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
			double angle = ROTATION_ANGLE;
			double scale = ISOTROPIC_SCALE_FACTOR;

			// Generate the Rotation Matrix

			rot_mat = getRotationMatrix2D( center, angle, scale );

			// Rotate the Image

			warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );


			/*------------------------- Storing the Image ---------------------------*/


			sprintf(frame_name3,"Affine_frame_no_%05u.ppm",i);

			// Storing the Image

			imwrite(frame_name3, warp_dst);

		}
		// End of 'for' loop

		printf("Affine Transform done!!\n"); 
	}
	// end 'if(option!=2)

	gettimeofday(&affine_end,NULL);
	affine_sec=affine_end.tv_sec-affine_start.tv_sec;
	affine_min=affine_sec/60;
	affine_sec=affine_sec%60;
	affine_msec=affine_end.tv_usec-affine_start.tv_usec;
	affine_msec/=1000;


	gettimeofday(&total_end,NULL);
	total_sec=total_end.tv_sec-total_start.tv_sec-5;
	total_min=total_sec/60;
	total_sec=total_sec%60;
	total_msec=total_end.tv_usec-total_start.tv_usec;
	total_msec/=1000;


	printf("\n-------------------------------------------------------------------------\n");	
	printf("|   Profiling Statistics   |    Minutes   |   Seconds    | Milliseconds |\n");
	printf("-------------------------------------------------------------------------\n");
	printf("| Sobel Transform Timings  |     %02d       |     %02d       |   %07d    |\n",sobel_min,sobel_sec,sobel_msec);
	printf("-------------------------------------------------------------------------\n");
	printf("| Affine Transform Timings |     %02d       |     %02d       |   %07d    |\n",affine_min,affine_sec,affine_msec);
	printf("-------------------------------------------------------------------------\n");
	printf("| Total Transform Timings  |     %02d       |     %02d       |   %07d    |\n",total_min,total_sec,total_msec);
	printf("-------------------------------------------------------------------------\n");

	return EXIT_SUCCESS;
}

int number_of_files()
{

	/*******************************************************//*
	* Note that this function returns the number of 
	* frames stored in the "Original Frames' folder.
	* Note that this version of the code computes and
	* generates the Sobel and Affine Frames in the 
	* same directory. 
	* IMPORTANT POINT: When the code
	* is executed after the 1st time, the function will 
	* return TOTAL NUMBER of frames, i.e., Original+
	* Sobel+Affine and NOT just ORIGINAL FRAMES.
	* Why use this function then?
	* Well, initially, we were having different folders
	* for Sobel, Affine and Original frames. So, this use
	* was perfect. However, for the timing analysis, there
	* we didn't want to waste time in changing directories
	* for scanning one frame and change again to store it 
	* in a different one.
	* So, what do we do for this code?
	* We can either hardcode the number of frames, as done
	* here,
	* OR
	* Make sure that the directory 'Original Frames' contains
	* only the extracted frames, and not the transformed ones
	* before executing the code after the 1st time.
	/*********************************************************/

	int file_count = 0;
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir("./Original_Frames"); 
	while ((entry = readdir(dirp)) != NULL) 
	{
		if (entry->d_type == DT_REG)	
		{
			file_count++;
		}
	}
	//printf("\nNumber of Frames: %d",file_count);
	closedir(dirp);
	return(file_count);

}
