package org.mippscoincore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class MippscoinQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File mippscoinDir = new File(getFilesDir().getAbsolutePath() + "/.mippscoin");
        if (!mippscoinDir.exists()) {
            mippscoinDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
