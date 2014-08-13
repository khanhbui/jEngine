package jk;

import jk.jEngine.R;
import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;



public class GameApplication extends Application {
	static GameApplication instance;
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		instance = this;
	}

	public static Context getContext() {
		// TODO Auto-generated method stub
		return instance;
	}
	
	public static void killApp(boolean killSafely) {
		if (killSafely) {
			/*
			 * Notify the system to finalize and collect all objects of the app
			 * on exit so that the virtual machine running the app can be killed
			 * by the system without causing issues. NOTE: If this is set to
			 * true then the virtual machine will not be killed until all of its
			 * threads have closed.
			 */
			System.runFinalizersOnExit(true);

			/*
			 * Force the system to close the app down completely instead of
			 * retaining it in the background. The virtual machine that runs the
			 * app will be killed. The app will be completely created as a new
			 * app in a new virtual machine running in a new process if the user
			 * starts the app again.
			 */
			System.exit(0);
		} else {
			/*
			 * Alternatively the process that runs the virtual machine could be
			 * abruptly killed. This is the quickest way to remove the app from
			 * the device but it could cause problems since resources will not
			 * be finalized first. For example, all threads running under the
			 * process will be abruptly killed when the process is abruptly
			 * killed. If one of those threads was making multiple related
			 * changes to the database, then it may have committed some of those
			 * changes but not all of those changes when it was abruptly killed.
			 */
			android.os.Process.killProcess(android.os.Process.myPid());
		}
	}

	public static void doRestart(Activity anyActivity) {
		Intent i = anyActivity.getPackageManager().getLaunchIntentForPackage(anyActivity.getPackageName());
		i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
		anyActivity.startActivity(i);
	}
	public void showWebPage(String url, Context from) {
		Intent i = new Intent(Intent.ACTION_VIEW);
		i.setData(Uri.parse(url));
		from.startActivity(i);
	}

	public void callNumber(String number, Context from) {
		Intent callIntent = new Intent(Intent.ACTION_CALL, Uri.parse("tel:" + number));
		from.startActivity(callIntent);
	}

	public void createHomeShortcut(Context context, Class className) {
		//Adding shortcut for MainActivity
		//on Home screen
		Intent shortcutIntent = new Intent(getApplicationContext(), className);

		shortcutIntent.setAction(Intent.ACTION_MAIN);

		Intent addIntent = new Intent();
		addIntent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
		addIntent.putExtra(Intent.EXTRA_SHORTCUT_NAME, getResources().getString(R.string.app_name));
		addIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, Intent.ShortcutIconResource.fromContext(getApplicationContext(), R.drawable.ic_launcher));

		addIntent.setAction("com.android.launcher.action.INSTALL_SHORTCUT");
		getApplicationContext().sendBroadcast(addIntent);
	}

	public static GameApplication getInstance() {
		return instance;
	}
}
