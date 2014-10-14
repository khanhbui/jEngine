/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code
#ifndef ANDROID
#define ANDROID
#endif

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Engine.h"
#include "GameEvent.h"
#include "MainScene.h"

int touches_x[10];
int touches_y[10];
int numOfTouches;

bool isReady = false;
bool hasPermission = true;
int width, height;
int mHightScore = 0;
bool mTutorial = false;

static const char * APP_PACKAGE_NAME = "jk.jEngine.swipeblocks";

static int now_ms(void)
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);

    return (((int)(1000.0 * res.tv_sec)) % 3200);
}

static double getCurrentMillSecond()
{
    struct timeval stCurrentTime;
    gettimeofday(&stCurrentTime,NULL);
    double lLastTime = (1000.0 * (stCurrentTime.tv_sec)) + (0.001 * (stCurrentTime.tv_usec));
    return lLastTime;
}

void onExit()
{
	isReady = false;
	if (j::Engine::Instance() != NULL)
	{
		j::Engine::Delete();
	}
}

bool setupGraphics(int w, int h)
{
	LOGD("setupGraphics");

	width = w;
	height = h;
	return true;
}

void renderFrame()
{
	if (isReady && hasPermission)
	{
		j::Engine::Instance()->enterFrame();
	}
}

void setAtlas(int aTextureID, const char* atlas, int len)
{
	LOGD("Atlas TextureID=%d",aTextureID);
	LOGD("Atlas Len=%d",len);
	LOGD("Atlas=%s",atlas);

	j::Engine::Create();
	j::Engine::Instance()->setup();
	j::Engine::Instance()->resize(width , height);
	j::Engine::Instance()->setTextureAtlas(aTextureID, atlas, len);
	j::Engine::Instance()->setDefaultScene(new MainScene(mTutorial));
	j::Engine::Instance()->onResume();

	isReady = true;
}

void onResume(int aTextureID, int oldTextureID)
{
	//Reinitialize OpenGL ES 2 window on resume
	if (aTextureID > -1)
	{
		j::Engine::Instance()->resetTexture(aTextureID, oldTextureID);
	}

	j::Engine::Instance()->onResume();
	isReady = true;
}

void onPause()
{
	j::Engine::Instance()->onPause();
}

void onStop()
{
	isReady = false;
}

void onTouchPressed(int x, int y)
{
	if (!isReady)
	{
		return;
	}

	j::Engine * engine = j::Engine::Instance();
	engine->numOfTouches = 0;

	int _touch_x = (int)((float)((float)(x - engine->renderer()->__offsetX) / (float)engine->renderer()->__fitWidth) * SCREEN_WIDTH);
	int _touch_y = (int)((float)((float)(y - engine->renderer()->__offsetY) / (float)engine->renderer()->__fitHeight) * SCREEN_HEIGHT);

	numOfTouches = 0;
	touches_x[numOfTouches] = _touch_x;
	touches_y[numOfTouches] = _touch_y;
	numOfTouches++;

	engine->setTouches(touches_x, touches_y, numOfTouches);
	engine->touchPressed(_touch_x, _touch_y);
}

void onTouchReleased(int x, int y)
{
	if (!isReady)
	{
		return;
	}

	j::Engine * engine = j::Engine::Instance();
	engine->numOfTouches = 0;


	int _touch_x = (int)((float)((float)(x - engine->renderer()->__offsetX) / (float)engine->renderer()->__fitWidth) * SCREEN_WIDTH);
	int _touch_y = (int)((float)((float)(y - engine->renderer()->__offsetY) / (float)engine->renderer()->__fitHeight) * SCREEN_HEIGHT);

	numOfTouches = 0;
	touches_x[numOfTouches] = -100;
	touches_y[numOfTouches] = -100;
	numOfTouches++;

	engine->setTouches(touches_x, touches_y, numOfTouches);
	engine->touchReleased(_touch_x, _touch_y, _touch_x, _touch_y);
}

void onTouchMoved(int src_x, int src_y, int x, int y)
{
	if (!isReady)
	{
		return;
	}

	j::Engine * engine = j::Engine::Instance();
	engine->numOfTouches = 0;

	int _touch_x = (int)((float)((float)(x - engine->renderer()->__offsetX) / (float)engine->renderer()->__fitWidth) * SCREEN_WIDTH);
	int _touch_y = (int)((float)((float)(y - engine->renderer()->__offsetY) / (float)engine->renderer()->__fitHeight) * SCREEN_HEIGHT);

	int _touch_src_x = (int)((float)((float)(src_x - engine->renderer()->__offsetX) / (float)engine->renderer()->__fitWidth) * SCREEN_WIDTH);
	int _touch_src_y = (int)((float)((float)(src_y - engine->renderer()->__offsetY) / (float)engine->renderer()->__fitHeight) * SCREEN_HEIGHT);

	numOfTouches = 0;
	touches_x[numOfTouches] = _touch_x;
	touches_y[numOfTouches] = _touch_y;
	numOfTouches++;

	engine->setTouches(touches_x, touches_y, numOfTouches);
	engine->touchMoved(_touch_src_x, _touch_src_y, _touch_x, _touch_y);
}

extern "C"
{
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_init(JNIEnv * env, jobject obj, jobject activity, jint width, jint height);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_exit(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_setAtlas(JNIEnv * env, jobject obj, jint aTextureID, jstring atlas, jint len);

    JNIEXPORT void JNICALL Java_jk_j_1JNILib_touchPressed(JNIEnv * env, jobject obj,  jint x, jint y);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_touchReleased(JNIEnv * env, jobject obj,  jint x, jint y);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_touchMoved(JNIEnv * env, jobject obj,  jint src_x, jint src_y, jint x, jint y);

    JNIEXPORT void JNICALL Java_jk_j_1JNILib_clearOutputEventCount(JNIEnv * env, jobject obj);
    JNIEXPORT jint JNICALL Java_jk_j_1JNILib_getOutputEventCount(JNIEnv * env, jobject obj);
    JNIEXPORT jint JNICALL Java_jk_j_1JNILib_getOutputEvent(JNIEnv * env, jobject obj, jint index);

    JNIEXPORT jint JNICALL Java_jk_j_1JNILib_getHighScore(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_setHighScore(JNIEnv * env, jobject obj,  jint hs);

    JNIEXPORT void JNICALL Java_jk_j_1JNILib_setTutorial(JNIEnv * env, jobject obj,  jboolean tutorial);

    JNIEXPORT void JNICALL Java_jk_j_1JNILib_resume(JNIEnv * env, jobject obj, jint aTextureID, jint oldTextureID);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_pause(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_jk_j_1JNILib_stop(JNIEnv * env, jobject obj);

    JNIEXPORT jboolean JNICALL Java_jk_j_1JNILib_isPlayingState(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_jk_j_1JNILib_init(JNIEnv * env, jobject obj, jobject activity, jint width, jint height)
{
	jclass android_content_Context =env->GetObjectClass(activity);

	jmethodID midGetPackageName = env->GetMethodID(android_content_Context, "getPackageName", "()Ljava/lang/String;");

	jstring packageName= (jstring)env->CallObjectMethod(activity, midGetPackageName);
	const char *aa = env->GetStringUTFChars(packageName, NULL);

	if (strcmp(aa, APP_PACKAGE_NAME) != 0)
	{
		hasPermission = false;
	}
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_exit(JNIEnv * env, jobject obj)
{
	onExit();
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_setAtlas(JNIEnv * env, jobject obj, jint aTextureID, jstring atlas, jint len)
{
	const char *aa = env->GetStringUTFChars(atlas, NULL);
	setAtlas(aTextureID, aa, len);
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_touchPressed(JNIEnv * env, jobject obj,  jint x, jint y)
{
	onTouchPressed(x, y);
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_touchReleased(JNIEnv * env, jobject obj,  jint x, jint y)
{
	onTouchReleased(x, y);
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_touchMoved(JNIEnv * env, jobject obj, jint src_x, jint src_y, jint x, jint y)
{
	onTouchMoved(src_x, src_y, x, y);
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_clearOutputEventCount(JNIEnv * env, jobject obj)
{
	j::Engine::Instance()->outputEventCount = 0;
}

JNIEXPORT jint JNICALL Java_jk_j_1JNILib_getOutputEventCount(JNIEnv * env, jobject obj)
{
	return j::Engine::Instance()->outputEventCount;
}

JNIEXPORT jint JNICALL Java_jk_j_1JNILib_getOutputEvent(JNIEnv * env, jobject obj, jint index)
{
	j::Engine * engine = j::Engine::Instance();

	jint ret = engine->outputEvents[index];

	return ret;
}

JNIEXPORT jint JNICALL Java_jk_j_1JNILib_getHighScore(JNIEnv * env, jobject obj)
{
	return j::Engine::Instance()->highScore;
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_setHighScore(JNIEnv * env, jobject obj,  jint hs){
	mHightScore = hs;
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_setTutorial(JNIEnv * env, jobject obj,  jboolean tutorial){
	mTutorial = tutorial;
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_resume(JNIEnv * env, jobject obj, jint aTextureID, jint oldTextureID)
{
	onResume(aTextureID, oldTextureID);
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_pause(JNIEnv * env, jobject obj)
{
	onPause();
}

JNIEXPORT void JNICALL Java_jk_j_1JNILib_stop(JNIEnv * env, jobject obj)
{
	onStop();
}

JNIEXPORT jboolean JNICALL Java_jk_j_1JNILib_isPlayingState(JNIEnv * env, jobject obj)
{
	return false;
}
