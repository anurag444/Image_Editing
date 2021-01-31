#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <android/log.h>
#include <android/bitmap.h>

#define  LOG_TAG    "libimageprocessing"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)



//extern "C" JNIEXPORT jstring JNICALL
//Java_com_example_imageediting_MainActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}

static void meanfilter(AndroidBitmapInfo* info, void* pixels){
    int xx, yy;
    std::vector<int>  redv, bluev, greenv;
    uint32_t* line;
    int sumred = 0;
    int sumblue = 0;
    int sumgreen = 0;



    for(yy = 0; yy <= info->height-3; yy++) {
        line = (uint32_t*)pixels;
        for (xx = 0; xx <= info->width-3; xx++) {

            int position = yy * info->width + xx;
            sumred = 0;
            sumblue = 0;
            sumgreen = 0;
            redv.clear();
            bluev.clear();
            greenv.clear();

            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"totaly  %d",info->height);
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"current  %d",yy);
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"totalx  %d",info->width);
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"current  %d",xx);

            bluev.push_back( (int) ((line[position] & 0x00FF0000) >> 16));
            greenv.push_back((int) ((line[position] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[position] & 0x00000FF )) ;

            bluev.push_back((int) ((line[yy*info->width+xx+1] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[yy*info->width+xx+1] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[yy*info->width+xx+1] & 0x00000FF )) ;

            bluev.push_back( (int) ((line[yy*info->width+xx+2] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[yy*info->width+xx+2] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[yy*info->width+xx+2] & 0x00000FF )) ;

            bluev.push_back( (int) ((line[(yy+1)*info->width+xx] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+1)*info->width+xx] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+1)*info->width+xx] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+1)*info->width+xx+1] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+1)*info->width+xx+1] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+1)*info->width+xx+1] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+1)*info->width+xx+2] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+1)*info->width+xx+2] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+1)*info->width+xx+2] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+2)*info->width+xx] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+2)*info->width+xx] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+2)*info->width+xx] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+2)*info->width+xx+1] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+2)*info->width+xx+1] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+2)*info->width+xx+1] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+2)*info->width+xx+2] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+2)*info->width+xx+2] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+2)*info->width+xx+2] & 0x00000FF ));

            for (int i = 0; i<9 ;i++){
                sumred = sumred + redv[i];
                sumblue= sumblue + bluev[i];
                sumgreen = sumgreen + greenv[i];
            }



            line[(yy+1)*info->width+xx+1] =
                    (((sumblue/9) << 16) & 0x00FF0000) |
                    (((sumgreen/9) << 8) & 0x0000FF00) |
                    ((sumred/9) & 0x000000FF);


        }
        pixels = (char*)pixels + info->stride;
    }
}

static void medianfilter(AndroidBitmapInfo* info, void* pixels){
    int xx, yy;
    std::vector<int>  redv, bluev, greenv;
    uint32_t* line;


    for(yy = 0; yy <= info->height-3; yy++) {
        line = (uint32_t*)pixels;
        for (xx = 0; xx <= info->width-3; xx++) {

            int position = yy * info->width + xx;
            redv.clear();
            bluev.clear();
            greenv.clear();

            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"totaly  %d",info->height);
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"current  %d",yy);
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"totalx  %d",info->width);
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"current  %d",xx);

            bluev.push_back( (int) ((line[position] & 0x00FF0000) >> 16));
            greenv.push_back((int) ((line[position] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[position] & 0x00000FF )) ;

            bluev.push_back((int) ((line[yy*info->width+xx+1] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[yy*info->width+xx+1] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[yy*info->width+xx+1] & 0x00000FF )) ;

            bluev.push_back( (int) ((line[yy*info->width+xx+2] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[yy*info->width+xx+2] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[yy*info->width+xx+2] & 0x00000FF )) ;

            bluev.push_back( (int) ((line[(yy+1)*info->width+xx] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+1)*info->width+xx] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+1)*info->width+xx] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+1)*info->width+xx+1] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+1)*info->width+xx+1] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+1)*info->width+xx+1] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+1)*info->width+xx+2] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+1)*info->width+xx+2] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+1)*info->width+xx+2] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+2)*info->width+xx] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+2)*info->width+xx] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+2)*info->width+xx] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+2)*info->width+xx+1] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+2)*info->width+xx+1] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+2)*info->width+xx+1] & 0x00000FF ));

            bluev.push_back( (int) ((line[(yy+2)*info->width+xx+2] & 0x00FF0000) >> 16));
            greenv.push_back((int)((line[(yy+2)*info->width+xx+2] & 0x0000FF00) >> 8)) ;
            redv.push_back((int) (line[(yy+2)*info->width+xx+2] & 0x00000FF ));

            sort(redv.begin(), redv.end());
            sort(bluev.begin(), bluev.end());
            sort(greenv.begin(), greenv.end());

            line[(yy+1)*info->width+xx+1] =
                    ((bluev[4] << 16) & 0x00FF0000) |
                    ((greenv[4] << 8) & 0x0000FF00) |
                    (redv[4] & 0x000000FF);


        }
        pixels = (char*)pixels + info->stride;
    }
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_imageediting_MainActivity_Jnimean(JNIEnv *env, jobject ZObject,jobject bitmap ) {
    AndroidBitmapInfo info;
    int ret;
    void *pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }

    meanfilter(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_imageediting_MainActivity_Jnimedian(JNIEnv *env, jobject ZObject,jobject bitmap ){
    AndroidBitmapInfo  info;
    int ret;
    void* pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return ;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return ;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return ;
    }

    medianfilter(&info,pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}
