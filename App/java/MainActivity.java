package Android.App.App;

import android.os.Bundle;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
// import android.view.WindowInsetsController.Behavior;
// import android.view.WindowManager;
import com.google.androidgamesdk.GameActivity;

public class MainActivity extends GameActivity {

	static { System.loadLibrary("App"); }

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		//hideSystemUI();
	}

	/*
	// Method to hide the system UI using WindowInsetsController
	private void hideSystemUI()
	{
		// Ensure the activity is set to full screen
		getWindow().setDecorFitsSystemWindows(false);

		// Get the WindowInsetsController
		WindowInsetsController insetsController = getWindow().getInsetsController();
		if (insetsController != null)
		{
			// Hide both the status bar and the navigation bar
			insetsController.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());

			// Set behavior to automatically hide system bars after a short interaction
			insetsController.setSystemBarsBehavior(Behavior.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
		}
	}

	// Method to show the system UI if needed
	private void showSystemUI()
	{
		getWindow().setDecorFitsSystemWindows(true);

		// Get the WindowInsetsController
		WindowInsetsController insetsController = getWindow().getInsetsController();
		if (insetsController != null)
		{
			// Show both the status bar and the navigation bar
			insetsController.show(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
		}
	}
	*/

	// TODO: Add Custom callback methods

}
