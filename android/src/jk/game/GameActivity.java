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

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;

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
import basegameutils.GameHelper;

public class GameActivity extends Activity implements GameHelper.GameHelperListener {
	private static final String LEADER_BOARD_ID = "CgkI98HjhKYdEAIQAQ";
	public static final String RSM_NAME = "jEngine";
	protected static final String TAG = "jENGINE";

	GameView mView;

	public SoundManager mSoundManager;
	
	private int mHighScore = 0;
	private int mPlayCount = 0;
	private int mRated = 0;
	private boolean mTutorial = false;

	private FrameLayout adContainer;
	public AdView adView;

	protected GameHelper mHelper;
	protected int mRequestedClients = GameHelper.CLIENT_GAMES;
	protected boolean mDebugLog = true;

	boolean mFirstTime = true;

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
		mTutorial = (boolean) prefs.getBoolean("tutorial", false);
		Log.i(TAG, "Retrieve tutorial mTutorial=" + mTutorial);

		saveRSM();

		final RelativeLayout relativeLayout = new RelativeLayout(this);
		final FrameLayout.LayoutParams relativeLayoutLayoutParams = new FrameLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
		final android.widget.RelativeLayout.LayoutParams surfaceViewLayoutParams = new RelativeLayout.LayoutParams(createSurfaceViewLayoutParams());
		surfaceViewLayoutParams.addRule(RelativeLayout.CENTER_IN_PARENT);

		relativeLayout.addView(this.mView, surfaceViewLayoutParams);

		adView = new AdView(this);
		adView.setAdUnitId("ca-app-pub-8223213496721349/2059545315");
		adView.setAdSize(AdSize.SMART_BANNER);
		adView.setVisibility(View.VISIBLE);

		adContainer = new FrameLayout(this);
		final LayoutParams layoutParams = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT, Gravity.TOP | Gravity.CENTER_HORIZONTAL);
		adContainer.addView(adView, layoutParams);

		relativeLayout.addView(adContainer);

		this.setContentView(relativeLayout, relativeLayoutLayoutParams);

		AdRequest adRequest = new AdRequest.Builder().addTestDevice(AdRequest.DEVICE_ID_EMULATOR).build();
		adView.loadAd(adRequest);

		if (mHelper == null) {
			getGameHelper();
		}
		mHelper.setMaxAutoSignInAttempts(1);//disable auto first sign in
		mHelper.setup(this);
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
	protected void onActivityResult(int request, int response, Intent data) {
		super.onActivityResult(request, response, data);
		mHelper.onActivityResult(request, response, data);
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

		showBackpressConfirm();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}

	public void processGameEvent() {
		int eventCount = j_JNILib.getOutputEventCount();
		if (eventCount > 0) 
		{
			for (int i = 0; i < eventCount; i++) 
			{
				int event = j_JNILib.getOutputEvent(i); 
				Log.d(TAG, String.format("Event %d = %d", i, event));

				switch (event) {
					case GameEvent.GAME_EVENT_MAINMENU: {
						this.displayAd();
	
						if (mFirstTime)
		            	{
			            	this.runOnUiThread(new Runnable() {
			        			@Override
			        			public void run() {
			        				try {
			        					Thread.sleep(500);
			        				} catch (Exception e) {}
			        				if (GameActivity.this.isSignedIn()) {
			        				} else {
			        					GameActivity.this.beginUserInitiatedSignIn();
			        				}
			        			}
			        		});
			            	mFirstTime = false;
		            	}
						break;
					}
	
					case GameEvent.GAME_EVENT_GAMEPLAY: {
						this.dismissAd();
						break;
					}
	
					case GameEvent.GAME_EVENT_GAMEOVER: {
						this.displayAd();
	
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
							
							if (!mTutorial) {
								mTutorial = true;
							}
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
						intent.setData(Uri.parse("market://details?id=jk.jEngine.swipeblocks"));
						this.startActivity(intent);
						break;
					}
					
					case GameEvent.ENGINE_EVENT_FINISH_TUTORIAL: {
						mTutorial = true;
						saveRSM();
						Log.i(TAG, "Save tutorial mTutorial=" + mTutorial);
						break;
					}
				}
			}
		}
		j_JNILib.clearOutputEventCount();
	}

	private void saveRSM() {
		SharedPreferences prefs = this.getSharedPreferences(RSM_NAME, Context.MODE_PRIVATE);
		Editor editor = prefs.edit();
		editor.clear();
		editor.putInt("score", mHighScore);
		editor.putInt("playcount", mPlayCount);
		editor.putInt("rated", mRated);
		editor.putBoolean("tutorial", mTutorial);
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
				new AlertDialog.Builder(GameActivity.this).setTitle("Quit Game").setMessage("Are you sure?").setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
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

	public void dismissAd() {
		Log.d(TAG, "jk.game.GameActivity.dismissAd");
		this.runOnUiThread(new Runnable() {
			public void run() {
				adView.setVisibility(AdView.GONE);
			}
		});
	}

	public void displayAd() {
		Log.d(TAG, "jk.game.GameActivity.displayAd");
		this.runOnUiThread(new Runnable() {
			public void run() {
				adView.setVisibility(AdView.VISIBLE);
			}
		});
	}

	public void showLeaderBoard() {
		Log.d(TAG, "jk.game.GameActivity.showLeaderBoard");
		this.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (GameActivity.this.isSignedIn()) {
					GameActivity.this.startActivityForResult(Games.Leaderboards.getLeaderboardIntent(GameActivity.this.getApiClient(), LEADER_BOARD_ID), 1);
				} else {
					GameActivity.this.beginUserInitiatedSignIn();
				}
			}
		});
	}

	public void submitScore(long score) {
		Log.d(TAG, "jk.game.GameActivity.submitScore");
		if (this.isSignedIn()) {
			Games.Leaderboards.submitScore(getApiClient(), LEADER_BOARD_ID, score);
		}
	}

	public int getHighScore() {
		return mHighScore;
	};
	
	public boolean getTutorial() {
		return mTutorial;
	};

	public GameHelper getGameHelper() {
		if (mHelper == null) {
			mHelper = new GameHelper(this, mRequestedClients);
			mHelper.enableDebugLog(mDebugLog);
		}
		return mHelper;
	}
	
	protected GoogleApiClient getApiClient() {
		return mHelper.getApiClient();
	}

	protected boolean isSignedIn() {
		return mHelper.isSignedIn();
	}

	protected void beginUserInitiatedSignIn() {
		mHelper.beginUserInitiatedSignIn();
	}

	protected void signOut() {
		mHelper.signOut();
	}

	protected void showAlert(String message) {
		mHelper.makeSimpleDialog(message).show();
	}

	protected void showAlert(String title, String message) {
		mHelper.makeSimpleDialog(title, message).show();
	}

	protected void enableDebugLog(boolean enabled) {
		mDebugLog = true;
		if (mHelper != null) {
			mHelper.enableDebugLog(enabled);
		}
	}

	protected String getInvitationId() {
		return mHelper.getInvitationId();
	}

	protected void reconnectClient() {
		mHelper.reconnectClient();
	}

	protected boolean hasSignInError() {
		return mHelper.hasSignInError();
	}

	protected GameHelper.SignInFailureReason getSignInError() {
		return mHelper.getSignInError();
	}

	@Override
	public void onSignInFailed() {
		Log.d(TAG, "jk.game.GameActivity.onSignInFailed");
	}

	@Override
	public void onSignInSucceeded() {
		Log.d(TAG, "jk.game.GameActivity.onSignInSucceeded");
	}
}
