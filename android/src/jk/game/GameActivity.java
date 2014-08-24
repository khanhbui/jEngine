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

package jk.game;

import jk.GameApplication;
import jk.j_JNILib;
import jk.jEngine.swipeblocks.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.amazon.device.ads.Ad;
import com.amazon.device.ads.AdError;
import com.amazon.device.ads.AdLayout;
import com.amazon.device.ads.AdProperties;
import com.amazon.device.ads.AdRegistration;
import com.amazon.device.ads.DefaultAdListener;

public class GameActivity extends Activity {
	private static boolean ENABLE_AMZN_SERVICE = false;

	public static final String APP_KEY = "02198f7beac647e699c116d30052f0ba";
	public static final int AD_REFESH_RATE = 30;
	public static final String LEADERBOARD_ID = "jEngine";

	public static final String RSM_NAME = "jEngine";

	static final String TAG = "jENGINE";
	GameView mView;

	public SoundManager mSoundManager;
	private int mHighScore = 0;
	private int mPlayCount = 0;
	private int mRated = 0;

	// Amazon Ads
	private FrameLayout adContainer;
	private AdLayout adView;
	private long lastTime = 0;

	TextView mTextInfo;
	
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		Log.i(TAG, "GameActivity.onCreate");
		mSoundManager = new SoundManager(this);
		mView = new GameView(getApplication(), this);

		SharedPreferences prefs = this.getSharedPreferences(RSM_NAME, Context.MODE_PRIVATE);
		mHighScore = (int) prefs.getInt("score", 0);
		mPlayCount = (int) prefs.getInt("playcount", 0);
		mRated = (int) prefs.getInt("rated", 0);

		mPlayCount++;
		saveRSM();

		final RelativeLayout relativeLayout = new RelativeLayout(this);
		final FrameLayout.LayoutParams relativeLayoutLayoutParams = new FrameLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
		final android.widget.RelativeLayout.LayoutParams surfaceViewLayoutParams = new RelativeLayout.LayoutParams(createSurfaceViewLayoutParams());
		surfaceViewLayoutParams.addRule(RelativeLayout.CENTER_IN_PARENT);

		relativeLayout.addView(this.mView, surfaceViewLayoutParams);

		if (ENABLE_AMZN_SERVICE) {
			AdRegistration.enableLogging(true);
			AdRegistration.enableTesting(true);
			try {
				AdRegistration.setAppKey(APP_KEY);
			} catch (final Exception e) {
				Log.e(TAG, "Exception thrown: " + e.toString());
				return;
			}

			adContainer = new FrameLayout(this);
			adView = new AdLayout(this);
			final LayoutParams layoutParams = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT, Gravity.TOP | Gravity.CENTER_HORIZONTAL);
			adView.setLayoutParams(layoutParams);
			adView.setListener(new AdListener());

			relativeLayout.addView(adContainer);
		}

		this.setContentView(relativeLayout, relativeLayoutLayoutParams);
	}

	protected static LayoutParams createSurfaceViewLayoutParams() {
		final LayoutParams layoutParams = new LayoutParams(android.view.ViewGroup.LayoutParams.MATCH_PARENT, android.view.ViewGroup.LayoutParams.MATCH_PARENT);
		layoutParams.gravity = Gravity.CENTER;
		return layoutParams;
	}

	@Override
	protected void onStart() {
		Log.d(TAG, "jk.game.GameActivity.onStart");
		super.onStart();
	}

	@Override
	protected void onStop() {
		super.onStop();
		Log.d(TAG, "jk.game.GameActivity.onStop");
		j_JNILib.stop();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.d(TAG, "jk.game.GameActivity.onDestroy");

		if (this.adView != null) {
			this.adView.destroy();
		}
	}

	@Override
	protected void onPause() {
		super.onPause();

		Log.d(TAG, "jk.game.GameActivity.onPause");
		mView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();

		Log.d(TAG, "jk.game.GameActivity.onResume");
		mView.onResume();
	}

	@Override
	public void onBackPressed() {
		Log.d(TAG, "jk.game.GameActivity.onBackPressed");

		super.onBackPressed();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}

	public void processGameEvent() {

		int eventCount = j_JNILib.getOutputEventCount();
		
		// Log.d(TAG, String.format("processGameEvent: Event Count = %d",
		// dot_JNILib.getOutputEventCount()));
		if (eventCount > 0) {
			int[] events = j_JNILib.getOutputEvents();
			for (int i = 0; i < eventCount; i++) {
				Log.d(TAG, String.format("Event %d = %d", i, events[i]));
				switch (events[i]) {
				case GameEvent.GAME_EVENT_MAINMENU: {
					this.DisplayAd();
					break;
				}

				case GameEvent.GAME_EVENT_GAMEPLAY: {
					this.DismissAd();
					break;
				}

				case GameEvent.GAME_EVENT_GAMEOVER: {
					this.DisplayAd();

					showRating();
					break;
				}

				case GameEvent.GAME_EVENT_SOUND_COIN:
					mSoundManager.play(SoundManager.SOUND_POINT);
					break;

				case GameEvent.ENGINE_EVENT_NEW_SCORE: {
					int score = j_JNILib.getHighScore();

					if (score > mHighScore) {
						Log.i(TAG, "Save HS score=" + score + " hs=" + mHighScore);

						mHighScore = score;
						this.submitScore(mHighScore);
						saveRSM();
					}
					break;
				}

				case GameEvent.ENGINE_EVENT_LEADERBOARD: {
					this.showLeaderBoard();
					break;
				}

				case GameEvent.ENGINE_EVENT_RATE_BUTTON: {
					Intent intent = new Intent(Intent.ACTION_VIEW);
					intent.setData(Uri.parse("market://details?id=jk.jEngine"));
					this.startActivity(intent);
					break;
				}
				}
			}
		}
	}

	private void saveRSM() {
		SharedPreferences prefs = this.getSharedPreferences(RSM_NAME, Context.MODE_PRIVATE);
		Editor editor = prefs.edit();
		editor.clear();
		editor.putInt("score", mHighScore);
		editor.putInt("playcount", mPlayCount);
		editor.putInt("rated", mRated);
		editor.commit();
	}

	private void showRating() {
		if (mRated == 0 && (mPlayCount == 5 || mPlayCount == 15 || mPlayCount == 30 || mPlayCount == 50 || mPlayCount == 100))
			this.runOnUiThread(new Runnable() {
				public void run() {
					mPlayCount++;// disable rating for current session
					new AlertDialog.Builder(GameActivity.this).setTitle("Rate jEngine").setMessage("Thank you for your support!").setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							saveRSM();

							Intent intent = new Intent(Intent.ACTION_VIEW);
							intent.setData(Uri.parse("market://details?id=jk.jEngine"));
							GameActivity.this.startActivity(intent);
						}
					}).setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {

						}
					}).setIcon(R.drawable.ic_launcher).show();
				}
			});
	}

	private void showBackpressConfirm() {
		this.runOnUiThread(new Runnable() {
			public void run() {
				Log.d(TAG, "Show backpressed confirm: paused game");
				j_JNILib.pause();
				new AlertDialog.Builder(GameActivity.this).setTitle("Quit Game").setMessage("Quit?").setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						j_JNILib.exit();
						GameApplication.killApp(true);
					}
				})
				.setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						j_JNILib.resume(-1, -1);
					}
				})
				.setOnCancelListener(new DialogInterface.OnCancelListener() {         
				    @Override
				    public void onCancel(DialogInterface dialog) {
				    	j_JNILib.resume(-1, -1);
				    }
				})
				.setIcon(R.drawable.ic_launcher).show();
			}
		});
	}

	public void DismissAd() {
		if (!ENABLE_AMZN_SERVICE) {
			return;
		}
		this.runOnUiThread(new Runnable() {
			public void run() {
				adView.setVisibility(View.INVISIBLE);
				Log.i(TAG, "DismissAd " + lastTime);
			}
		});
	}

	public void DisplayAd() {
		if (!ENABLE_AMZN_SERVICE) {
			return;
		}
		this.runOnUiThread(new Runnable() {
			public void run() {
				long now = System.currentTimeMillis() / 1000L;
				Log.i(TAG, "DisplayAd " + (now - lastTime) + ", " + j_JNILib.isPlayingState());
				if (now - lastTime >= AD_REFESH_RATE) {
					lastTime = now;
					adContainer.removeView(adView);
					adView.loadAd();
				} else {
					adView.setVisibility(View.VISIBLE);
				}
			}
		});
	}

	public void showLeaderBoard() {
		Log.d(TAG, "jk.game.GameActivity.showLeaderBoard");
	}

	public void submitScore(long score) {
		Log.d(TAG, "jk.game.GameActivity.submitScore");
	}

	class AdListener extends DefaultAdListener {
		@Override
		public void onAdLoaded(final Ad ad, final AdProperties adProperties) {
			Log.i(TAG, adProperties.getAdType().toString() + " ad loaded successfully.");

			GameActivity.this.lastTime = System.currentTimeMillis() / 1000L;
			GameActivity.this.adContainer.addView(GameActivity.this.adView);

			if (!j_JNILib.isPlayingState()) {
				GameActivity.this.adView.setVisibility(View.VISIBLE);
			}
		}

		@Override
		public void onAdFailedToLoad(final Ad ad, final AdError error) {
			Log.w(TAG, "Ad failed to load. Code: " + error.getCode() + ", Message: " + error.getMessage());
			GameActivity.this.adView.setVisibility(View.INVISIBLE);
		}

		@Override
		public void onAdExpanded(final Ad ad) {
			Log.i(TAG, "Ad expanded.");
		}

		@Override
		public void onAdCollapsed(final Ad ad) {
			Log.i(TAG, "Ad collapsed.");
		}
	}

	public int getHighScore() {
		return mHighScore;
	};
}
