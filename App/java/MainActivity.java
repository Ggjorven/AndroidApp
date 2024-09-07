package Android.App.App;

import android.os.Bundle;

import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;

import com.google.androidgamesdk.GameActivity;

public class MainActivity extends GameActivity {

	static { System.loadLibrary("App"); }

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		hideSystemUI();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		hideSystemUI();
	}

	// TODO: Add Custom callback methods

	// Custom methods
	private void hideSystemUI()
	{
		WindowInsetsController controller = getWindow().getInsetsController();
		if (controller != null)
		{
			/*
			View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_FULLSCREEN);
			*/
			controller.hide(
				WindowInsets.Type.statusBars() |
				WindowInsets.Type.navigationBars() |
				WindowInsets.Type.systemOverlays() |
				WindowInsets.Type.captionBar() |
				WindowInsets.Type.systemBars()
			);

			controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
		}
	}

}
