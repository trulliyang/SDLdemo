/*
 * consumer_xgl.c
 * Copyright (C) 2012 Christophe Thommeret
 * Author: Christophe Thommeret <hftom@free.fr>
 * Based on Nehe's GLX port by Mihael.Vrbanec@stud.uni-karlsruhe.de
 * http://nehe.gamedev.net/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#define GL_GLEXT_PROTOTYPES
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/log.h>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#include <framework/mlt.h>
#include <SDL.h>
#include <include/SDL_opengl.h>
#include <assert.h>


#define STARTWIDTH 1280
#define STARTHEIGHT 720


extern int XInitThreads();


void  check_errorr() {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        assert(0);
    }

}

typedef struct consumer_xgl_s *consumer_xgl;

struct consumer_xgl_s
{
	struct mlt_consumer_s parent;
	mlt_properties properties;
	mlt_deque queue;
	pthread_t thread;
	int joined;
	int running;
	int playing;
	int xgl_started;
};


typedef struct
{
	pthread_t thread;
	int running;
} thread_video;


typedef struct
{
	int width;
	int height;
	double aspect_ratio;
	GLuint texture;
	pthread_mutex_t mutex;
	int new;
	mlt_frame mlt_frame_ref;
} frame_new;


typedef struct
{
	int width;
	int height;
	GLuint fbo;
	GLuint texture;
} fbo;


typedef struct
{
    int screen;
	SDL_Window* win;
	SDL_GLContext ctx;
} HiddenContext;


typedef struct
{
    int screen;
	SDL_Window* win;
	SDL_GLContext ctx;
//    XSetWindowAttributes attr;
    int x, y;
    unsigned int width, height;
    unsigned int depth;
} GLWindow;


static GLWindow GLWin;
static HiddenContext hiddenctx;

static frame_new new_frame;
static fbo fb;
static thread_video vthread;
static consumer_xgl xgl;
static mlt_filter glsl_manager;



static void* video_thread( void *arg );

static void update()
{
	int _width = GLWin.width;
	int _height = GLWin.height;
	GLfloat left, right, top, bottom;
	GLfloat war = (GLfloat)_width/(GLfloat)_height;

	if ( war < new_frame.aspect_ratio ) {
		left = -1.0;
		right = 1.0;
		top = war / new_frame.aspect_ratio;
		bottom = -war / new_frame.aspect_ratio;
	}
	else {
		top = 1.0;
		bottom = -1.0;
		left = -new_frame.aspect_ratio / war;
		right = new_frame.aspect_ratio / war;
	}

    glClearColor(0,1,0,1);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    check_errorr();
//	glLoadIdentity();
//    check_errorr();
//
//	glPushMatrix();
//    check_errorr();


//    glTranslatef( _width/2, _height/2, 0 );
//    check_errorr();
//
//    glScalef( _width/2, _height/2, 1.0 );
//    check_errorr();


    glBindTexture( GL_TEXTURE_2D, fb.texture );
    check_errorr();

/*
	glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( left, top );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( left, bottom );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( right, bottom );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( right, top );
	glEnd();
*/
	GLfloat vtx1[] = {
			left, top , 0.0f,
			left, bottom ,0.0f,
			right, bottom , 0.0f,
			right, top , 0.0f
	};

	GLfloat tex1[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
	};

	glEnableClientState(GL_VERTEX_ARRAY);
    check_errorr();

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    check_errorr();

    glVertexPointer(3, GL_FLOAT, 0, vtx1);
    check_errorr();

    glTexCoordPointer(2, GL_FLOAT, 0, tex1);
    check_errorr();

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    check_errorr();


    glDisableClientState(GL_VERTEX_ARRAY);
    check_errorr();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    check_errorr();

//    glPopMatrix();
//    check_errorr();


    //TODO
    SDL_GL_SwapWindow(GLWin.win);

//	SDL_UpdateWindowSurface(GLWin.win);
//	glXSwapBuffers( GLWin.dpy, GLWin.win );
	
//	if ( !vthread.running ) {
//		pthread_create( &vthread.thread, NULL, video_thread, NULL );
//		vthread.running = 1;
//	}
}

static const char gVertexShader[] = "attribute vec4 vPosition;\n"
		"void main() {\n"
		"  gl_Position = vPosition;\n"
		"}\n";

static const char gFragmentShader[] = "precision mediump float;\n"
		"void main() {\n"
		"  gl_FragColor = vec4(0.0, 1.0, 0.0, 0.5);\n"
		"}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					fprintf(stderr, "Could not compile shader %d:\n%s\n",
							shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error
													= glGetError()) {
		fprintf(stderr, "after %s() glError (0x%x)\n", op, error);
	}
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					fprintf(stderr, "Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

GLuint gProgram = 0;
GLuint gvPositionHandle = 0;
GLuint gTextureProgram = 0;
GLuint gvTexturePositionHandle = 0;
GLuint gvTextureTexCoordsHandle = 0;
GLuint gvTextureSamplerHandle = 0;

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
									  0.5f, -0.5f };

const GLint FLOAT_SIZE_BYTES = 4;
const GLint TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;


//const GLfloat gTriangleVerticesData[] = {
//        // X, Y, Z, U, V
//        -1.0f, -1.0f, 0, 0.f, 0.f,
//        1.0f, -1.0f, 0, 1.f, 0.f,
//        -1.0f,  1.0f, 0, 0.f, 1.f,
//        1.0f,   1.0f, 0, 1.f, 1.f,
//};
const GLfloat gTriangleVerticesData[] = {
        // X, Y, Z, U, V
        -1.0f, -1.0f, 0, 0.f, 1.0f,
        1.0f, -1.0f, 0, 1.f, 1.0f,
        -1.0f,  1.0f, 0, 0.f, 0.0f,
        1.0f,   1.0f, 0, 1.f, 0.0f,
};

static const char gSimpleVS[] =
        "attribute vec4 position;\n"
                "attribute vec2 texCoords;\n"
                "varying vec2 outTexCoords;\n"
                "\nvoid main(void) {\n"
                "    outTexCoords = texCoords;\n"
                "    gl_Position = position;\n"
                "}\n\n";
static const char gSimpleFS[] =
        "precision mediump float;\n\n"
                "varying vec2 outTexCoords;\n"
                "uniform sampler2D texture;\n"
                "\nvoid main(void) {\n"
                "    gl_FragColor = texture2D(texture, outTexCoords);\n"
                "}\n\n";

static struct timeval showTime;
static int    f = 0;
static void show_frame()
{

	if(f == 0){
		f = 1;
		gettimeofday(&showTime,NULL);
	}
	else{
		struct timeval Tmp;
		gettimeofday(&Tmp,NULL);

		int64_t  ll = (Tmp.tv_sec * 1000 +  Tmp.tv_usec /1000) -  (showTime.tv_sec * 1000 +  showTime.tv_usec /1000);
		showTime = Tmp;
//        if(ll > 34)
//		__android_log_print(ANDROID_LOG_ERROR, "ProjectName", "need a frame render mlt_consumer_rt_frame  interval %lld !!!" , ll);
	}


	SDL_GL_MakeCurrent(GLWin.win,GLWin.ctx);

    gTextureProgram = createProgram(gSimpleVS, gSimpleFS);
    if (!gTextureProgram) {
        return ;
    }
    gvTexturePositionHandle = glGetAttribLocation(gTextureProgram, "position");
    checkGlError("glGetAttribLocation");
    gvTextureTexCoordsHandle = glGetAttribLocation(gTextureProgram, "texCoords");
    checkGlError("glGetAttribLocation");
    gvTextureSamplerHandle = glGetUniformLocation(gTextureProgram, "texture");
    checkGlError("glGetAttribLocation");

//    glViewport( 0, 0, new_frame.width, new_frame.height );
	glViewport( 0, 0, 1920, 1080 );
//        glViewport( 0, 0,1280, 720 );

    check_errorr();

    glBindTexture(GL_TEXTURE_2D, new_frame.texture);

    // Back to the display
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGlError("glBindFramebuffer");

    // Draw copied content on the screen
    glUseProgram(gTextureProgram);
    checkGlError("glUseProgram");

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glVertexAttribPointer(gvTexturePositionHandle, 3, GL_FLOAT, GL_FALSE,
                          TRIANGLE_VERTICES_DATA_STRIDE_BYTES, gTriangleVerticesData);
    checkGlError("glVertexAttribPointer");
    glVertexAttribPointer(gvTextureTexCoordsHandle, 2, GL_FLOAT, GL_FALSE,
                          TRIANGLE_VERTICES_DATA_STRIDE_BYTES, &gTriangleVerticesData[3]);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvTexturePositionHandle);
    glEnableVertexAttribArray(gvTextureTexCoordsHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    SDL_GL_SwapWindow(GLWin.win);
    mlt_events_fire( MLT_CONSUMER_PROPERTIES(&xgl->parent), "consumer-frame-show", new_frame.mlt_frame_ref, NULL );
    check_errorr();

    mlt_frame_close( new_frame.mlt_frame_ref );
    check_errorr();

    new_frame.mlt_frame_ref = NULL;
    new_frame.new = 0;

    return ;
	/*if(gProgram == 0){
		gProgram = createProgram(gVertexShader, gFragmentShader);
		if (!gProgram) {
			return ;
		}
	}

	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	checkGlError("glGetAttribLocation");

	// Bind FBO and draw into it
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	checkGlError("glBindFramebuffer");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	checkGlError("glClearColor");
	glClear(GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	glUseProgram(gProgram);
	checkGlError("glUseProgram");

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(gvPositionHandle);
	checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	checkGlError("glDrawArrays");
	 */


//	SDL_GL_SwapWindow(GLWin.win);

    glBindFramebuffer( GL_FRAMEBUFFER, fb.fbo );
    check_errorr();


    glViewport( 0, 0, new_frame.width, new_frame.height );
    check_errorr();


    glClearColor(1.0, 0, 0, 1.0f);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glActiveTexture( GL_TEXTURE0 );
    check_errorr();

    glBindTexture( GL_TEXTURE_2D, new_frame.texture );
    check_errorr();

    GLfloat vtx1[] = {
            0.0f, 0.0f , 0.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f , 0.0f,
            1.0f, 0.0f, 0.0f
    };

    GLfloat tex1[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    check_errorr();

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    check_errorr();


    glVertexPointer(3, GL_FLOAT, 0, vtx1);
    check_errorr();

    glTexCoordPointer(2, GL_FLOAT, 0, tex1);
    check_errorr();

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    check_errorr();


    glDisableClientState(GL_VERTEX_ARRAY);
    check_errorr();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    check_errorr();



    SDL_GL_SwapWindow(GLWin.win);

	new_frame.new = 0;

    return ;
#if  0

    if ( (fb.width != new_frame.width) || (fb.height != new_frame.height) ) {
		glDeleteFramebuffers( 1, &fb.fbo );
        check_errorr();

        glDeleteTextures( 1, &fb.texture );
        check_errorr();

        fb.fbo = 0;
		fb.width = new_frame.width;
		fb.height = new_frame.height;
		glGenFramebuffers( 1, &fb.fbo );
        check_errorr();

        glGenTextures( 1, &fb.texture );
        check_errorr();

        glBindTexture( GL_TEXTURE_2D, fb.texture );
        check_errorr();

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, fb.width, fb.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
        check_errorr();

        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        check_errorr();

        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        check_errorr();

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        check_errorr();

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        check_errorr();

        glBindFramebuffer( GL_FRAMEBUFFER, fb.fbo );
        check_errorr();

        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.texture, 0 );
        check_errorr();

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        check_errorr();

    }

//	glPushAttrib(GL_VIEWPORT_BIT);
//    glMatrixMode(GL_PROJECTION);
//    check_errorr();
//
//    glPushMatrix();
//    check_errorr();


    glBindFramebuffer( GL_FRAMEBUFFER, fb.fbo );
    check_errorr();


    glViewport( 0, 0, new_frame.width, new_frame.height );
    check_errorr();

//    glMatrixMode( GL_PROJECTION );
//    check_errorr();

//    glLoadIdentity();
//    check_errorr();
//
//    glOrthof( 0.0, new_frame.width, 0.0, new_frame.height, -1.0, 1.0 );
//    check_errorr();
//
//    glMatrixMode( GL_MODELVIEW );
//    check_errorr();
//
//    glLoadIdentity();
//    check_errorr();


    glActiveTexture( GL_TEXTURE0 );
    check_errorr();

    glBindTexture( GL_TEXTURE_2D, new_frame.texture );
    check_errorr();


    /*
    glBegin( GL_QUADS );
        glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 0.0f, 0.0f );
        glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 0.0f, new_frame.height );
        glTexCoord2f( 1.0f, 1.0f ); glVertex2f( new_frame.width, new_frame.height );
        glTexCoord2f( 1.0f, 0.0f ); glVertex2f( new_frame.width, 0.0f );
    glEnd();
*/
	GLfloat vtx1[] = {
			0.0f, 0.0f , 0.0f,
			0.0f, new_frame.height, 0.0f,
			new_frame.width, new_frame.height , 0.0f,
			new_frame.width, 0.0f, 0.0f
	};

	GLfloat tex1[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
	};

	glEnableClientState(GL_VERTEX_ARRAY);
    check_errorr();

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    check_errorr();


    glVertexPointer(3, GL_FLOAT, 0, vtx1);
    check_errorr();

    glTexCoordPointer(2, GL_FLOAT, 0, tex1);
    check_errorr();

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    check_errorr();


    glDisableClientState(GL_VERTEX_ARRAY);
    check_errorr();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    check_errorr();

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    check_errorr();

    mlt_events_fire( MLT_CONSUMER_PROPERTIES(&xgl->parent), "consumer-frame-show", new_frame.mlt_frame_ref, NULL );
    check_errorr();

    mlt_frame_close( new_frame.mlt_frame_ref );
    check_errorr();

    new_frame.mlt_frame_ref = NULL;

//	glMatrixMode(GL_PROJECTION);
//    check_errorr();
//
//    glPopMatrix();
//    check_errorr();
//
//    glMatrixMode(GL_MODELVIEW);
//    check_errorr();

//    glPopAttrib();
	
	update();
	
	new_frame.new = 0;
#endif
}

static void createGLWindow();
static void killGLWindow();

void* video_thread( void *arg )
{
	mlt_frame next = NULL;
	mlt_consumer consumer = &xgl->parent;
	mlt_properties consumer_props = MLT_CONSUMER_PROPERTIES( consumer );
	struct timeval start, end;
	double duration = 0;

    createGLWindow();

	gettimeofday( &start, NULL );

	while ( vthread.running )
	{
		// Get a frame from the attached producer
		next = mlt_consumer_rt_frame( consumer );

		if ( !mlt_properties_get_int( MLT_FILTER_PROPERTIES( glsl_manager ), "glsl_supported" ) ) {
			mlt_log_error( MLT_CONSUMER_SERVICE(consumer), "OpenGL Shading Language is not supported on this machine.\n" );
			xgl->running = 0;
			break;
		}

		// Ensure that we have a frame
		if ( next )
		{
			mlt_properties properties =  MLT_FRAME_PROPERTIES( next );
			if ( mlt_properties_get_int( properties, "rendered" ) == 1 )
			{
				// Get the image, width and height
				mlt_image_format vfmt = mlt_image_glsl_texture;
				int width = 0, height = 0;
				GLuint *image = 0;
				int error = mlt_frame_get_image( next, (uint8_t**) &image, &vfmt, &width, &height, 0 );
				if ( !error && image && width && height && !new_frame.new ) {
					new_frame.width = width;
					new_frame.height = height;
					new_frame.texture = *image;
					new_frame.mlt_frame_ref = next;
					new_frame.aspect_ratio = ((double)width / (double)height) * mlt_properties_get_double( properties, "aspect_ratio" );
					new_frame.new = 1;

					show_frame();
					int loop = 200;
//					while ( new_frame.new && --loop )
//						usleep( 500 );
				}
				else
				{
					mlt_frame_close( next );
				}
				new_frame.new = 0;
				
				gettimeofday( &end, NULL );
				duration = 1000000.0 / mlt_properties_get_double( consumer_props, "fps" );
				duration -= ( end.tv_sec * 1000000 + end.tv_usec ) - ( start.tv_sec * 1000000 + start.tv_usec );
				if ( duration > 0 )
					usleep( (int)duration );
				gettimeofday( &start, NULL );
			}
			else
			{
				mlt_frame_close( next );
				static int dropped = 0;

//                __android_log_print(ANDROID_LOG_ERROR, "ProjectName", "need a frame render mlt_consumer_rt_frame dropped %d !!!",dropped);

                mlt_log_info( MLT_CONSUMER_SERVICE(consumer), "dropped video frame %d\n", ++dropped );
			}
		}
		else
			usleep( 1000 );
	}
	if(xgl->xgl_started)
		killGLWindow();
	mlt_consumer_stopped( consumer );
	
	return NULL;
}



static void resizeGLScene()
{
	//TODO
	//glXMakeCurrent( GLWin.dpy, GLWin.win, GLWin.ctx );
	SDL_GL_MakeCurrent(GLWin.win,GLWin.ctx);
	
	if ( GLWin.height == 0 )
		GLWin.height = 1;
	if ( GLWin.width == 0 )
		GLWin.width = 1;
	glViewport( 0, 0, GLWin.width, GLWin.height );
    check_errorr();
//	glMatrixMode( GL_PROJECTION );
//	check_errorr();

//	glLoadIdentity();
//	check_errorr();

//	glOrthof( 0.0, GLWin.width, 0.0, GLWin.height, -1.0, 1.0 );
//	check_errorr();
//
//	glMatrixMode( GL_MODELVIEW );
//	check_errorr();


	update();
}



static void initGL( void )
{
	//TODO
	//glXMakeCurrent( GLWin.dpy, GLWin.win, GLWin.ctx );
	SDL_GL_MakeCurrent(GLWin.win,GLWin.ctx);

//	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//    check_errorr();
//	glClearDepthf( 1.0f );
//    check_errorr();

//    glDepthFunc( GL_LEQUAL );
//    check_errorr();
//
//    glEnable( GL_DEPTH_TEST );
//    check_errorr();
//
//    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//    check_errorr();
//
//    glEnable( GL_BLEND );
//    check_errorr();
//
//    glShadeModel( GL_SMOOTH );
//    check_errorr();

//    glEnable( GL_TEXTURE_2D );
//    check_errorr();

//    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//    check_errorr();


    //TODO
//	typedef int (*GLXSWAPINTERVALSGI) ( int );
//	GLXSWAPINTERVALSGI mglXSwapInterval = (GLXSWAPINTERVALSGI)glXGetProcAddressARB( (const GLubyte*)"glXSwapIntervalSGI" );
//	if ( mglXSwapInterval )
//		mglXSwapInterval( 1 );

	fb.fbo = 0;
	fb.width = STARTWIDTH;
	fb.height = STARTHEIGHT;
	glGenFramebuffers( 1, &fb.fbo );
    check_errorr();

    glGenTextures( 1, &fb.texture );
    check_errorr();

    glBindTexture( GL_TEXTURE_2D, fb.texture );
    check_errorr();

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, fb.width, fb.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
    check_errorr();

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    check_errorr();

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    check_errorr();

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    check_errorr();

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    check_errorr();

    glBindFramebuffer( GL_FRAMEBUFFER, fb.fbo );
    check_errorr();

    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb.texture, 0 );
    check_errorr();

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    check_errorr();


    resizeGLScene();
}



static void createGLWindow()
{
	const char* title = "OpenGL consumer";
	int width = STARTWIDTH;
	int height = STARTHEIGHT;

	/*
	int attrListSgl[] = { GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8, GLX_DEPTH_SIZE, 16, None };

	int attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_DEPTH_SIZE, 16, None };

	XVisualInfo *vi;
	Colormap cmap;
	Atom wmDelete;
	Window winDummy;
	unsigned int borderDummy;

	GLWin.dpy = XOpenDisplay( 0 );
	GLWin.screen = DefaultScreen( GLWin.dpy );

	vi = glXChooseVisual( GLWin.dpy, GLWin.screen, attrListDbl );
	if ( !vi )
		vi = glXChooseVisual( GLWin.dpy, GLWin.screen, attrListSgl );

	GLWin.ctx = glXCreateContext( GLWin.dpy, vi, 0, GL_TRUE );

	cmap = XCreateColormap( GLWin.dpy, RootWindow( GLWin.dpy, vi->screen ), vi->visual, AllocNone );
	GLWin.attr.colormap = cmap;
	GLWin.attr.border_pixel = 0;

	GLWin.attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;
	GLWin.win = XCreateWindow( GLWin.dpy, RootWindow(GLWin.dpy, vi->screen), 0, 0, width, height,
		0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &GLWin.attr );
	wmDelete = XInternAtom( GLWin.dpy, "WM_DELETE_WINDOW", True );
	XSetWMProtocols( GLWin.dpy, GLWin.win, &wmDelete, 1 );
	XSetStandardProperties( GLWin.dpy, GLWin.win, title, title, None, NULL, 0, NULL );
	XMapRaised( GLWin.dpy, GLWin.win );

	glXMakeCurrent( GLWin.dpy, GLWin.win, GLWin.ctx );
	XGetGeometry( GLWin.dpy, GLWin.win, &winDummy, &GLWin.x, &GLWin.y,
		&GLWin.width, &GLWin.height, &borderDummy, &GLWin.depth );


	// Verify GLSL works on this machine
	hiddenctx.ctx = glXCreateContext( GLWin.dpy, vi, GLWin.ctx, GL_TRUE );
	if ( hiddenctx.ctx ) {
		hiddenctx.dpy = GLWin.dpy;
		hiddenctx.screen = GLWin.screen;
		hiddenctx.win = RootWindow( hiddenctx.dpy, hiddenctx.screen );
	}
	*/

	int ret = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE);

	GLWin.win = SDL_CreateWindow("mlt sdl xgl window",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  width, height,
										  SDL_WINDOW_OPENGL);
//	if(GLWin.win == NULL){
//		assert(0);
//	}
    GLWin.height = height;
    GLWin.width = width;
	GLWin.ctx = SDL_GL_CreateContext(GLWin.win);
//	initGL();
}



static void killGLWindow()
{		
//	if ( GLWin.ctx ) {
//		if ( !glXMakeCurrent( GLWin.dpy, None, NULL ) ) {
//			printf("Error releasing drawing context : killGLWindow\n");
//		}
//		glXDestroyContext( GLWin.dpy, GLWin.ctx );
//		GLWin.ctx = NULL;
//	}
//
//	if ( hiddenctx.ctx )
//		glXDestroyContext( hiddenctx.dpy, hiddenctx.ctx );
//
//	XCloseDisplay( GLWin.dpy );
	if(GLWin.ctx){
		SDL_GL_DeleteContext(GLWin.ctx);
		GLWin.ctx = 0;
	}

	if(GLWin.win){
		SDL_DestroyWindow(GLWin.win);
		GLWin.win = NULL;
	}
}



static void run()
{
	//XEvent event;

	while ( xgl->running ) {
	/*	while ( XPending( GLWin.dpy ) > 0 ) {
			XNextEvent( GLWin.dpy, &event );
			switch ( event.type ) {
				case Expose:
					if ( event.xexpose.count != 0 )
						break;
					break;
				case ConfigureNotify:
					if ( (event.xconfigure.width != GLWin.width) || (event.xconfigure.height != GLWin.height) ) {
						GLWin.width = event.xconfigure.width;
						GLWin.height = event.xconfigure.height;
						resizeGLScene();
					}
					break;
				case KeyPress:
					switch ( XLookupKeysym( &event.xkey, 0 ) ) {
						case XK_Escape:									
							xgl->running = 0;
							break;
						default: {
							mlt_producer producer = mlt_properties_get_data( xgl->properties, "transport_producer", NULL );
							char keyboard[ 2 ] = " ";
							void (*callback)( mlt_producer, char * ) = mlt_properties_get_data( xgl->properties, "transport_callback", NULL );
							if ( callback != NULL && producer != NULL )
							{
								keyboard[ 0 ] = ( char )XLookupKeysym( &event.xkey, 0 );
								callback( producer, keyboard );
							}
							break;
						}
					}
					break;
				case ClientMessage:
					if ( *XGetAtomName( GLWin.dpy, event.xclient.message_type ) == *"WM_PROTOCOLS" )
						xgl->running = 0;
					break;
				default:
					break;
			}
		}
		*/
		//TODO
//		if ( new_frame.new )
//			show_frame();
//		else
//			usleep( 1000 );
	}
}



void start_xgl( consumer_xgl consumer )
{
	xgl = consumer;
	
	pthread_mutex_init( &new_frame.mutex, NULL );
	new_frame.aspect_ratio = 16.0 / 9.0;
	new_frame.new = 0;
	new_frame.width = STARTWIDTH;
	new_frame.height = STARTHEIGHT;
	new_frame.mlt_frame_ref = NULL;
	
	vthread.running = 0;
	xgl->xgl_started = 1;

    if ( !vthread.running ) {
        pthread_create( &vthread.thread, NULL, video_thread, NULL );
        vthread.running = 1;
    }

//	createGLWindow();
//	SDL_GL_MakeCurrent(GLWin.win,GLWin.ctx);
//	mlt_events_fire( MLT_FILTER_PROPERTIES(glsl_manager), "init glsl", NULL );
	run();
	if ( vthread.running ) {
		vthread.running = 0;
		pthread_join( vthread.thread, NULL );
	}
	xgl->running = 0;
}


static struct ANativeWindow* window_;
static EGLDisplay display_;
static EGLSurface surface_;
static EGLContext context_;
static EGLConfig config_;
static int screen_width_ = 16;
static int screen_height_= 16;

static  int  create_offscreen_context()
{

    display_ = EGL_NO_DISPLAY;
    surface_ = EGL_NO_SURFACE;
    context_ = EGL_NO_CONTEXT;
    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display_, 0, 0);

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {EGL_RENDERABLE_TYPE,
                              EGL_OPENGL_ES2_BIT,  // Request opengl ES2.0
                              EGL_SURFACE_TYPE,
                              EGL_WINDOW_BIT,
                              EGL_BLUE_SIZE,
                              8,
                              EGL_GREEN_SIZE,
                              8,
                              EGL_RED_SIZE,
                              8,
                              EGL_DEPTH_SIZE,
                              24,
                              EGL_NONE};
    int color_size_ = 8;
    int depth_size_ = 24;

    EGLint num_configs;
    eglChooseConfig(display_, attribs, &config_, 1, &num_configs);

    if (!num_configs) {
        // Fall back to 16bit depth buffer
        const EGLint attribs[] = {EGL_RENDERABLE_TYPE,
                                  EGL_OPENGL_ES2_BIT,  // Request opengl ES2.0
                                  EGL_SURFACE_TYPE,
                                  EGL_WINDOW_BIT,
                                  EGL_BLUE_SIZE,
                                  8,
                                  EGL_GREEN_SIZE,
                                  8,
                                  EGL_RED_SIZE,
                                  8,
                                  EGL_DEPTH_SIZE,
                                  16,
                                  EGL_NONE};
        eglChooseConfig(display_, attribs, &config_, 1, &num_configs);
        depth_size_ = 16;
    }

    if (!num_configs) {
       // LOGW("Unable to retrieve EGL config");
        return -1;
    }

    EGLint PBufAttribs[] =
            {
                    EGL_WIDTH,  screen_width_,
                    EGL_HEIGHT, screen_width_,
                    EGL_LARGEST_PBUFFER, EGL_TRUE,
                    EGL_NONE
            };

//    surface_ = eglCreateWindowSurface(display_, config_, window_, NULL);
    surface_ = eglCreatePbufferSurface(display_, config_, PBufAttribs);
    if ( surface_ == EGL_NO_SURFACE )
    {
        return -1;
    }
    eglQuerySurface(display_, surface_, EGL_WIDTH, &screen_width_);
    eglQuerySurface(display_, surface_, EGL_HEIGHT, &screen_height_);

    const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION,
                                      2,  // Request opengl ES2.0
                                      EGL_NONE};

    context_ = eglCreateContext(display_, config_, (EGLContext)GLWin.ctx, context_attribs);

    if (eglMakeCurrent(display_, surface_, surface_, context_) == EGL_FALSE) {
//        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

//    context_valid_ = true;
//    return true;
    return 0;
}

void release_offscreen_context(){
	if (display_ != EGL_NO_DISPLAY) {
		eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context_ != EGL_NO_CONTEXT) {
			eglDestroyContext(display_, context_);
		}

		if (surface_ != EGL_NO_SURFACE) {
			eglDestroySurface(display_, surface_);
		}
		eglTerminate(display_);
	}

	display_ = EGL_NO_DISPLAY;
	context_ = EGL_NO_CONTEXT;
	surface_ = EGL_NO_SURFACE;
}


static void on_consumer_thread_started( mlt_properties owner, HiddenContext* context )
{
	// Initialize this thread's OpenGL state
	//TODO
//	createGLWindow();

/*

   GLWin.win = SDL_CreateWindow("mlt sdl xgl window",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 1280, 720,
                                 SDL_WINDOW_OPENGL);
//	if(GLWin.win == NULL){
//		assert(0);
//	}
    GLWin.ctx = SDL_GL_CreateContext(GLWin.win);*/
    create_offscreen_context();
//	release_offscreen_context();
//    GLuint dst_texnum, fbo;

//    glGenTextures(1, &dst_texnum);
//    int ii = glGetError();
//	glXMakeCurrent( context->dpy, context->win, context->ctx );
	//SDL_GL_MakeCurrent(GLWin.win,GLWin.ctx);
    mlt_events_fire( MLT_FILTER_PROPERTIES(glsl_manager), "init glsl", NULL );

}


static void on_consumer_thread_stopped( mlt_properties owner, HiddenContext* context ){
    mlt_events_fire( MLT_FILTER_PROPERTIES(glsl_manager), "close glsl", NULL );

    release_offscreen_context();
}
/** Forward references to static functions.
*/

static int consumer_start( mlt_consumer parent );
static int consumer_stop( mlt_consumer parent );
static int consumer_is_stopped( mlt_consumer parent );
static void consumer_close( mlt_consumer parent );
static void *consumer_thread( void * );



/** This is what will be called by the factory - anything can be passed in
	via the argument, but keep it simple.
*/

mlt_consumer consumer_xgl_init( mlt_profile profile, mlt_service_type type, const char *id, char *arg )
{
	// Create the consumer object
	consumer_xgl this = calloc( sizeof( struct consumer_xgl_s ), 1 );

	// If no malloc'd and consumer init ok
	if ( this != NULL && mlt_consumer_init( &this->parent, this, profile ) == 0 )
	{
		// Create the queue
		this->queue = mlt_deque_init( );

		// Get the parent consumer object
		mlt_consumer parent = &this->parent;

		// We have stuff to clean up, so override the close method
		parent->close = consumer_close;

		// get a handle on properties
		mlt_service service = MLT_CONSUMER_SERVICE( parent );
		this->properties = MLT_SERVICE_PROPERTIES( service );

		// Default scaler
		mlt_properties_set( this->properties, "rescale", "bilinear" );
		mlt_properties_set( this->properties, "deinterlace_method", "onefield" );

		// default image format
		mlt_properties_set( this->properties, "mlt_image_format", "glsl" );

		// Default buffer for low latency
		mlt_properties_set_int( this->properties, "buffer", 1 );

		// Ensure we don't join on a non-running object
		this->joined = 1;
		this->xgl_started = 0;

		// Allow thread to be started/stopped
		parent->start = consumer_start;
		parent->stop = consumer_stop;
		parent->is_stopped = consumer_is_stopped;

		//TODO
		// "init glsl" is required to instantiate glsl filters.
		glsl_manager = mlt_factory_filter( profile, "glsl.manager", NULL );
		if ( glsl_manager ) {
			mlt_events_listen( this->properties, &hiddenctx, "consumer-thread-started", (mlt_listener) on_consumer_thread_started );
			mlt_events_listen( this->properties, &hiddenctx, "consumer-thread-stopped", (mlt_listener) on_consumer_thread_stopped );

		} else {
			mlt_consumer_close( parent );
			parent = NULL;
		}

		// Return the consumer produced
		return parent;
	}

	// malloc or consumer init failed
	free( this );

	// Indicate failure
	return NULL;
}



int consumer_start( mlt_consumer parent )
{
	consumer_xgl this = parent->child;

	if ( !this->running )
	{
		consumer_stop( parent );

		this->running = 1;
		this->joined = 0;

		pthread_create( &this->thread, NULL, consumer_thread, this );
	}

	return 0;
}



int consumer_stop( mlt_consumer parent )
{
	// Get the actual object
	consumer_xgl this = parent->child;
	
	if ( this->running && this->joined == 0 )
	{
		// Kill the thread and clean up
		this->joined = 1;
		this->running = 0;

		if ( this->thread )
			pthread_join( this->thread, NULL );
	}

	return 0;
}



int consumer_is_stopped( mlt_consumer parent )
{
	consumer_xgl this = parent->child;
	return !this->running;
}



static void *consumer_thread( void *arg )
{
	// Identify the arg
	consumer_xgl this = arg;

//	XInitThreads();
	start_xgl( this );

	return NULL;
}



/** Callback to allow override of the close method.
*/

static void consumer_close( mlt_consumer parent )
{
	// Get the actual object
	consumer_xgl this = parent->child;

	// Stop the consumer
	///mlt_consumer_stop( parent );
	mlt_filter_close( glsl_manager );

	// Now clean up the rest
	mlt_consumer_close( parent );

	// Close the queue
	mlt_deque_close( this->queue );
//
//	if ( this->xgl_started )
//		killGLWindow();

	// Finally clean up this
	free( this );
}
