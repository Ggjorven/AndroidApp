package Horizon;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class Main extends Activity
{
	public static native int NativeMain();

	static 
	{
		System.loadLibrary("App");
	}

	@Override
	protected void onCreate(Bundle arg0) 
	{
		super.onCreate(arg0);

		NativeMain();
	}
}
