package jk;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.util.Log;

public class j_TextureAtlas

{

	public int mTextureID;

	public int width;

	public int height;

	Context context;

	public j_TextureAtlas(Context context, int ImageResourceId)

	{
		this.context = context;
		bindTexture(ImageResourceId);
	}

	public void bindTexture(int ImageResourceId)

	{
		InputStream stream = this.context.getResources().openRawResource(

		ImageResourceId);

		Bitmap bitmap = BitmapFactory.decodeStream(stream);

		int[] textureId = new int[1];

		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(bitmap.getWidth()

		* bitmap.getHeight() * 4);

		byteBuffer.order(ByteOrder.BIG_ENDIAN);

		IntBuffer ib = byteBuffer.asIntBuffer();

		int[] pixels = new int[bitmap.getWidth() * bitmap.getHeight()];

		bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(),

		bitmap.getHeight());

		for (int i = 0; i < pixels.length; i++)

		{

			ib.put(pixels[i] << 8 | pixels[i] >>> 24);

		}

		width = bitmap.getWidth();

		height = bitmap.getHeight();

		Log.d("libgl2jni", "Bitmap width=" + width + " height=" + height);
		bitmap.recycle();

		byteBuffer.position(0);

		GLES20.glGenTextures(1, textureId, 0);

		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId[0]);

		GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, bitmap.getWidth(), bitmap.getHeight(), 0, GLES20.GL_RGBA,

		GLES20.GL_UNSIGNED_BYTE, byteBuffer);

		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);

		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

		mTextureID = textureId[0];

	}
	
	public String getAtlas(int atlasId){
		InputStream is = this.context.getResources().openRawResource(atlasId);
        byte[] buffer;
		try {
			buffer = new byte[is.available()];
			while (is.read(buffer) != -1);
	        String text = new String(buffer);
	        return text;
		} catch (IOException e) {
		}
        return null;
	}
}