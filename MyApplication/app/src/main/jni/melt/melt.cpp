#include <Mlt.h>
#include <MltConsumer.h>
#include <MltRepository.h>
#include <3rd/LeakTracer-master/libleaktracer/include/MemoryTrace.hpp>
#include <unistd.h>
#include <jni.h>
#include "../mlt++/Mlt.h"
#include <android/log.h>
#include "mlt_log.h"


using namespace Mlt;

static Repository *m_repo = NULL;

void factory_init() {
	// should call mltsetenv before this init
	static int factory_inited = 0;
	if (0 == factory_inited) {
		m_repo = Factory::init();
		factory_inited = 1;
	}
}

#define MAXFILTERS 512
char *filterlist[MAXFILTERS] = {0};

extern "C" mlt_properties mlt_repository_filters( mlt_repository self );

Mlt::FilteredConsumer* ffconsumer=NULL;
int consume_stop = 0;

int query_filters(char **filternames, int maxcnt){
	int i = 0;
	int loop = 0;
	int count = 0;
	char * m;
	char **w = filternames;
	factory_init();
	Properties *filters = m_repo->filters();
	loop = filters->count()> maxcnt?maxcnt:filters->count();

	for(i = i;i<loop;i++){
		m = filters->get_name(i);
		*w = filters->get_name(i);
		w++;
	}
	return loop;
}

int test_filters_transitions(char *filter_name, char *video_name0, char *video_name1) {
	factory_init();
	Mlt::Profile m_pprofile;
	Mlt::Profile m_cprofile;

	m_pprofile.set_explicit(0);

	Mlt::Producer m_producer0(m_pprofile, "avformat", video_name0);

	m_producer0.set("put_mode", 0);
	m_producer0.set("real_time", 1);
	m_producer0.set("terminate_on_pause", 0);

	Mlt::Producer m_producer1(m_pprofile, "avformat", video_name1);

	m_producer1.set("put_mode", 0);
	m_producer1.set("real_time", 1);
	m_producer1.set("terminate_on_pause", 0);

	Mlt::FilteredConsumer m_consumer(m_pprofile, "xgl", NULL);

	m_consumer.set("width", 1920);
	m_consumer.set("height", 1072);

	Mlt::Filter m_filter(m_pprofile, "movit.convert");
	if (m_filter.is_valid()) {
		m_producer0.lock();
		m_producer0.attach(m_filter);
		m_producer0.unlock();
	}

	Mlt::Filter m_filter1(m_pprofile, "movit.mirror");
	if (m_filter1.is_valid()) {
		m_producer0.lock();
		m_producer0.attach(m_filter1);
		m_producer0.unlock();
	}

	Mlt::Filter m_filter2(m_pprofile, "movit.convert");
	if (m_filter2.is_valid()) {
		m_producer1.lock();
		m_producer1.attach(m_filter2);
		m_producer1.unlock();
	}

	Mlt::Filter m_filter3(m_pprofile, "movit.mirror");
	if (m_filter3.is_valid()) {
		m_producer1.lock();
		m_producer1.attach(m_filter3);
		m_producer1.unlock();
	}

	Mlt::Tractor m_tractor(m_pprofile);
	if (m_tractor.is_valid()) {
		m_tractor.lock();
		m_tractor.connect_producer(m_producer0, 0);
		m_tractor.connect_producer(m_producer1, 1);
		m_tractor.set_track(m_producer0, 0);
		m_tractor.set_track(m_producer1, 1);
		m_tractor.unlock();
	}

	Mlt::Transition m_transition(m_pprofile, "movit.overlay", NULL);
	if (m_transition.is_valid()) {
		m_tractor.lock();
		m_transition.set("always_active", "1");
		m_transition.set("accepts_blanks", "1");
		m_tractor.plant_transition(m_transition, 0, 1);
		m_transition.set_in_and_out(0, 5000);
		m_tractor.unlock();
	}

//	Mlt::Filter m_filter2(m_pprofile, "movit.lift_gamma_gain");
//	if(m_filter1.is_valid()){
//		/*
//        m_filter.set("av.x","200");
//        m_filter.set("av.y","10");
//        m_filter.set("av.w","300");
//        m_filter.set("av.h","300");
//        */
////		m_filter.set("_movit.parms.float.radius",112.4);
//
//		m_producer.lock();
//		m_producer.attach(m_filter2);
//		m_producer.unlock();
//	}

//	Mlt::Filter m_filter3(m_pprofile, "movit.glow");
//	if(m_filter1.is_valid()){
//		/*
//        m_filter.set("av.x","200");
//        m_filter.set("av.y","10");
//        m_filter.set("av.w","300");
//        m_filter.set("av.h","300");
//        */
////		m_filter.set("_movit.parms.float.radius",112.4);
//
//		m_producer.lock();
//		m_producer.attach(m_filter3);
//		m_producer.unlock();
//	}

	m_consumer.connect(m_tractor);
	m_consumer.run();

	end:
//    Factory::close();
	m_repo = NULL;
	return 0;

}

int test_filters(char *filter_name, char *video_name0, char *video_name1) {
	factory_init();
	Mlt::Profile m_pprofile;
	Mlt::Profile m_cprofile;

	m_pprofile.set_explicit(0);
//	Mlt::Producer m_producer(m_pprofile, video_name);

    Mlt::Producer m_producer(m_pprofile, "avformat", video_name0);

    m_producer.set("put_mode", 0);
	m_producer.set("real_time", 1);
	m_producer.set("terminate_on_pause", 0);

	Mlt::FilteredConsumer m_consumer(m_pprofile, "xgl" ,NULL);

    int t_h = m_pprofile.height();
    int t_w = m_pprofile.width();
//	t_w = 1280;
//	t_h = 720;
    __android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang video w=%d, h=%d", t_w, t_h);


	m_consumer.set("width", t_w);
	m_consumer.set("height", t_h);

	ffconsumer = &m_consumer;

	Mlt::Filter m_filter(m_pprofile, "movit.convert");
	if (m_filter.is_valid()) {
		m_producer.lock();
		m_producer.attach(m_filter);
		m_producer.unlock();
	} else {
        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filter movit.convert is invalid");
    }
    
//    Mlt::Filter m_filterCrop(m_pprofile, "movit.crop");
//    if(m_filterCrop.is_valid()){
//        m_producer.lock();
//        m_producer.attach(m_filterCrop);
//        m_producer.unlock();
//    } else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filter1 movit.crop is invalid");
//    }
    
//    Mlt::Filter m_filterResize(m_pprofile, "movit.resize");
//    if (m_filterResize.is_valid()) {
//        m_producer.lock();
//        m_producer.attach(m_filterResize);
//        m_producer.unlock();
//    } else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterResize movit.resize is invalid");
//    }

//    Mlt::Filter m_filterOverlay(m_pprofile, "movit.overlay");
//    if(m_filterOverlay.is_valid()){
//        m_producer.lock();
//        m_producer.attach(m_filterOverlay);
//        m_producer.unlock();
//    } else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filter1 movit.overlay is invalid");
//    }

//	Mlt::Filter m_filter2(m_pprofile, "movit.mirror");
//	if(m_filter2.is_valid()){
//		m_producer.lock();
//		m_producer.attach(m_filter2);
//		m_producer.unlock();
//	} else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filter2 movit.mirror is invalid");
//    }

//    Mlt::Filter m_filterVignette(m_pprofile, "movit.vignette");
//    if(m_filterVignette.is_valid()){
//        m_producer.lock();
//        m_producer.attach(m_filterVignette);
//        m_producer.unlock();
//    } else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterVignette movit.vignette is invalid");
//    }

//    Mlt::Filter m_filterSlice(m_pprofile, "movit.slice");
//    if(m_filterSlice.is_valid()){
//        m_producer.lock();
//        m_producer.attach(m_filterSlice);
//        m_producer.unlock();
//    } else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterSlice movit.slice is invalid");
//    }

//    Mlt::Filter m_filterGray(m_pprofile, "movit.gray");
//    if(m_filterGray.is_valid()){
//        m_producer.lock();
//        m_producer.attach(m_filterGray);
//        m_producer.unlock();
//    } else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterGray movit.gray is invalid");
//    }

//    Mlt::Filter m_filterBlur(m_pprofile, "movit.blur");
//    if(m_filterBlur.is_valid()){
//        m_producer.lock();
//        m_producer.attach(m_filterBlur);
//        m_producer.unlock();
//    } else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterBlur movit.blur is invalid");
//    }

//	Mlt::Filter m_filterOldCinema(m_pprofile, "movit.old_cinema");
//	if (m_filterOldCinema.is_valid()) {
//		m_producer.lock();
//		m_producer.attach(m_filterOldCinema);
//		m_producer.unlock();
//	} else {
//		__android_log_print(ANDROID_LOG_ERROR, "shiyang",
//                            "m_filterOldCinema movit.old_cinema is invalid");
//	}

//	Mlt::Filter m_filterVignette(m_pprofile, "movit.vignette");
//	if(m_filterVignette.is_valid()){
//		m_producer.lock();
//		m_producer.attach(m_filterVignette);
//		m_producer.unlock();
//	} else {
//		__android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterVignette movit.vignette is invalid");
//	}
	
//	Mlt::Filter m_filterStyleSketch(m_pprofile, "movit.style_sketch");
//	if (m_filterStyleSketch.is_valid()) {
//		m_producer.lock();
//		m_producer.attach(m_filterStyleSketch);
//		m_producer.unlock();
//	} else {
//		__android_log_print(ANDROID_LOG_ERROR, "shiyang",
//							"m_filterStyleSketch movit.style_sketch is invalid");
//	}
	
//	Mlt::Filter m_filterChromaKey(m_pprofile, "movit.chroma_key");
//	if (m_filterChromaKey.is_valid()) {
//		m_producer.lock();
//		m_producer.attach(m_filterChromaKey);
//		m_producer.unlock();
//	} else {
//		__android_log_print(ANDROID_LOG_ERROR, "shiyang",
//							"m_filterChromaKey movit.chroma_key is invalid");
//	}
	
	Mlt::Filter m_filterAlphaMatting(m_pprofile, "movit.alpha_matting");
	if (m_filterAlphaMatting.is_valid()) {
		m_producer.lock();
		m_producer.attach(m_filterAlphaMatting);
		m_producer.unlock();
	} else {
		__android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterAlphaMatting movit.alpha_matting is invalid");
	}
	
//	Mlt::Filter m_filterPixelation(m_pprofile, "movit.pixelation");
//	if(m_filterPixelation.is_valid()){
//		m_producer.lock();
//		m_producer.attach(m_filterPixelation);
//		m_producer.unlock();
//	} else {
//		__android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterPixelation movit.pixelation is invalid");
//	}

//	Mlt::Filter m_filterRect(m_pprofile, "movit.rect");
//	if(m_filterRect.is_valid()){
//		m_producer.lock();
//		m_producer.attach(m_filterRect);
//		m_producer.unlock();
//	} else {
//		__android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filterRect movit.rect is invalid");
//	}
    
    Mlt::Filter m_filterCrop(m_pprofile, "movit.crop");
    if(m_filterCrop.is_valid()){
        m_producer.lock();
        m_producer.attach(m_filterCrop);
        m_producer.unlock();
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_filter1 movit.crop is invalid");
    }
    
	m_consumer.connect(m_producer);
	m_consumer.run();
    
    __android_log_print(ANDROID_LOG_ERROR, "shiyang", "m_consumer.run() over");
    
	sleep(1);
	consume_stop = 1;
//	end:

    Factory::close();
	m_repo = NULL;
	return 0;
}

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void Java_org_libsdl_app_SDLActivity_nativeclose(){
	if(ffconsumer != NULL){
		ffconsumer->stop();
	}
	while(!consume_stop)
		usleep(10000);

//leaktracer::MemoryTrace::GetInstance().stopAllMonitoring();
	sleep(2);
//    LOGI("To writeLeaksToFile %s.", "/sdcard/leaks.out");
//	leaktracer::MemoryTrace::GetInstance().writeLeaksToFile("/sdcard/leaks.out");
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define main SDL_main

extern char source_path[1024];

int test_filters_c(char *filter_name, char *video_name0, char *video_name1){
	return test_filters(filter_name, video_name0, video_name1);
}

int test_filters_transitions_c(char *filter_name, char *video_name0, char *video_name1) {
	return test_filters_transitions(filter_name, video_name0, video_name1);
}

int query_filters_c(char **filters, int maxcnt){
	return query_filters(filters,maxcnt);
}

#include <android/log.h>

//static void new_callback( void* p, int l, const char* ll , va_list  lll){
//	__android_log_vprint(ANDROID_LOG_INFO, "123455", ll, lll);
//
//}

int main( int argc, char **argv ){
//    char *name0 = (char *) "mnt/sdcard/bbc-japan_1080p.mp4";
//	char *name0 = (char *) "mnt/sdcard/syred.mp4";
//	char *name0 = (char *) "mnt/sdcard/sygreen.mp4";
//    char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_151919.mp4";
//    char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_152043.mp4";
//    char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_151956.mp4";
//	char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_153223.mp4";//dog, good
//	char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_153659.mp4";//sheep
//	char *name0 = (char *) "mnt/sdcard/DCIM/sygreendog.mp4";
//	char *name0 = (char *) "mnt/sdcard/sygwall.mp4";
//    char *name0 = (char *) "mnt/sdcard/1_C_original.mp4";
//	char *name0 = (char *) "mnt/sdcard/greenlamp.mp4";
//	char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_164554.mp4";
//	char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_171921.mp4";
//	char *name0 = (char *) "mnt/sdcard/DCIM/Camera/20180213_171947.mp4";
    char *name0 = (char *) "mnt/sdcard/sygreen00001.mp4";
    char *name1 = (char *) "mnt/sdcard/dengziqi.mp4";
	
	char *nameg0 = (char *) "mnt/sdcard/lmjs.mp4";
	char *nameg1 = (char *) "mnt/sdcard/lmyt.mp4";
    char *nameg2 = (char *) "mnt/sdcard/lmsp1.mp4";
    char *nameg3 = (char *) "mnt/sdcard/lmyd.mp4";
    char *nameg4 = (char *) "mnt/sdcard/lmsp2.mp4";
    char *nameg5 = (char *) "mnt/sdcard/lmyw.mp4";
	
	char *namegs0 = (char *) "mnt/sdcard/lmsc/gs9_1.mp4";
	char *namegs1 = (char *) "mnt/sdcard/lmsc/gs9_2.mp4";
	char *namegs2 = (char *) "mnt/sdcard/lmsc/gs9_3.mp4";
	char *namegs3 = (char *) "mnt/sdcard/lmsc/gs9_4.mp4";
	char *namegs4 = (char *) "mnt/sdcard/lmsc/gs9_5.mp4";
	char *namegs5 = (char *) "mnt/sdcard/lmsc/gs9_6.mp4";
	char *namegs6 = (char *) "mnt/sdcard/lmsc/Fire Explosion Effect - green screen 9_7.mp4";
	
	char *namef0 = (char *) "mnt/sdcard/greenlamp.mp4";
	char *namef1 = (char *) "mnt/sdcard/1_C_original.mp4";
//	  mlt_log_set_callback(new_callback);
    //leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
//	FILE *fp = NULL;
//	char *fname = nameg3;
//	fp = fopen(fname, "r");
//	if (NULL == fp) {
//		__android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang open file %s failed", fname);
//	} else {
//        __android_log_print(ANDROID_LOG_ERROR, "shiyang", "shiyang file %s exists", fname);
//		fclose(fp);
//	}
	
    test_filters_c(NULL, name0, namegs0);
//	  test_filters_transitions_c(NULL, name0, name1);
//	  test_filters_c(NULL, (char *) "mnt/sdcard/sdltest.mp4");
//    test_filters_c(NULL, (char *) "mnt/sdcard/DCIM/Camera/20171012_062910.mp4");
//    test_filters_c(NULL, (char *) "mnt/sdcard/DCIM/Camera/20180124_112850.mp4");

	return 1;
}
#ifdef __cplusplus
}
#endif