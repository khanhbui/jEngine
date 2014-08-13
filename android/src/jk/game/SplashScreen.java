package jk.game;

import jk.jEngine.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;


public class SplashScreen extends Activity {

	// Splash screen timer
	private static int SPLASH_TIME_OUT = 2000;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_splash);

		Log.d(GameActivity.TAG, "jk.game.SplashActivity.onCreate");

		new Handler().postDelayed(new Runnable() {

			/*
			 * Showing splash screen with a timer. This will be useful when you
			 * want to show case your app logo / company
			 */

			public void run() {
				// close this activity
				finish();
				// This method will be executed once the timer is over
				// Start your app main activity
				Intent i = new Intent(SplashScreen.this, GameActivity.class);
				startActivity(i);

			}
		}, SPLASH_TIME_OUT);
	}

}
