/*
 * Copyright (C) 2007 The Android Open Source Project
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

package jk;

import jk.game.GameActivity;

// Wrapper for native library

public class j_JNILib {

     static {
         System.loadLibrary("jEngine");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(GameActivity context, int width, int height);
     public static native void startWithMainScene();
     public static native void exit();
     public static native void step();
     public static native void setAtlas(int id, String atlas, int len);
	 public static native void touchPressed(int x, int y);
	 public static native void touchReleased(int x, int y);

	 public static native void clearOutputEventCount();
	 public static native int getOutputEventCount();
	 public static native int getOutputEvent(int index);

	 public static native boolean isPlayingState();

	 public static native int getHighScore();
	 public static native void setHighScore(int hs);
	 
	 public static native void setTutorial(boolean tutorial);

	 public static native void resume(int textureID, int oldTextureID);
	 public static native void pause();
	 public static native void stop();
}
