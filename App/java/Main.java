package Android.App.App;

import android.os.Bundle;

import com.google.androidgamesdk.GameActivity;

public class Main extends GameActivity
{

	static { System.loadLibrary("App"); }

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
	}

}

