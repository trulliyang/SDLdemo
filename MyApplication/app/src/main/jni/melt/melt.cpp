#include <Mlt.h>
#include <MltConsumer.h>
#include <MltRepository.h>
#include <3rd/LeakTracer-master/libleaktracer/include/MemoryTrace.hpp>
#include <unistd.h>
#include <jni.h>
#include "../mlt++/Mlt.h"

#include "mlt_log.h"


using namespace Mlt;

static Repository *m_repo = NULL;

void factory_init() {
	// should call mltsetenv before this init
	static int factory_inited = 0;
	if(factory_inited == 0){
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

int test_filters(char *filter_name, char *video_name) {
	factory_init();
	Mlt::Profile m_pprofile;
	Mlt::Profile m_cprofile;

	m_pprofile.set_explicit(0);
//	Mlt::Producer m_producer(m_pprofile, video_name);

    Mlt::Producer m_producer(m_pprofile, "avformat",video_name);

    m_producer.set("put_mode", 0);
	m_producer.set("real_time", 1);
	m_producer.set("terminate_on_pause", 0);

	Mlt::FilteredConsumer m_consumer(m_pprofile,"xgl",NULL);



	m_consumer.set("width",1920);
	m_consumer.set("height",1072);


	ffconsumer = &m_consumer;

//	Mlt::Filter m_filter(m_pprofile, "grayscale");

	Mlt::Filter m_filter(m_pprofile, "movit.convert");
	if(m_filter.is_valid()){
		/*
        m_filter.set("av.x","200");
        m_filter.set("av.y","10");
        m_filter.set("av.w","300");
        m_filter.set("av.h","300");
		*/
		m_producer.lock();
		m_producer.attach(m_filter);
		m_producer.unlock();
	}



//    Mlt::Filter m_filter1(m_pprofile, "movit.crop");
//    if(m_filter1.is_valid()){
//
//
//        m_producer.lock();
//        m_producer.attach(m_filter1);
//        m_producer.unlock();
//    }


	Mlt::Filter m_filter2(m_pprofile, "movit.mirror");
	if(m_filter2.is_valid()){


		m_producer.lock();
		m_producer.attach(m_filter2);
		m_producer.unlock();
	}


	m_consumer.connect(m_producer);
	m_consumer.run();

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

//#define main    SDL_main

extern char source_path[1024];

int test_filters_c(char *filter_name, char *video_name){
	return test_filters(filter_name, video_name);
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
//	mlt_log_set_callback(new_callback);
    //leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
	test_filters_c(NULL, (char *) "mnt/sdcard/bbc-japan_1080p.mp4");
//	  test_filters_c(NULL, (char *) "mnt/sdcard/sdltest.mp4");
//    test_filters_c(NULL, (char *) "mnt/sdcard/DCIM/Camera/20171012_062910.mp4");
//    test_filters_c(NULL, (char *) "mnt/sdcard/DCIM/Camera/20180124_112850.mp4");

	return 1;
}
#ifdef __cplusplus
}
#endif