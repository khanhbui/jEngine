package jk.game;

import jk.GameApplication;
import jk.jEngine.swipeblocks.R;
import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;
import android.util.SparseIntArray;


public class SoundManager {
	public static final Integer SOUND_POINT = Integer.valueOf(3);

	private SoundPool mSoundPool;
	private AudioManager mAudioManager;
	private SparseIntArray mSoundsMap;
	private float mStreamVolume;
	Context mContext;

	public SoundManager(Context aContext) {
		this.mContext = aContext;
		mSoundPool = new SoundPool(10, AudioManager.STREAM_MUSIC, 100);
		mAudioManager = (AudioManager) GameApplication.getContext().getSystemService(Context.AUDIO_SERVICE);

		mSoundsMap = new SparseIntArray();
		mSoundsMap.put(SOUND_POINT, mSoundPool.load(mContext, R.raw.sfx_point, 1));
	}

	public void play(Integer soundID) {
		Log.i("SOUND", String.format("Play Sound %d", soundID));
		mStreamVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
		if(soundID == SOUND_POINT) {
			mStreamVolume /= 3;
		}
		mStreamVolume = mStreamVolume / mAudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
		
		mSoundPool.play(mSoundsMap.get(soundID), mStreamVolume, mStreamVolume, 1, 0, 1f);
	}
}
