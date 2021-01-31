package com.example.imageediting;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.content.FileProvider;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.ContentResolver;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.webkit.MimeTypeMap;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.bumptech.glide.Glide;
import com.wangjie.rapidfloatingactionbutton.RapidFloatingActionButton;
import com.wangjie.rapidfloatingactionbutton.RapidFloatingActionHelper;
import com.wangjie.rapidfloatingactionbutton.RapidFloatingActionLayout;
import com.wangjie.rapidfloatingactionbutton.contentimpl.labellist.RFACLabelItem;
import com.wangjie.rapidfloatingactionbutton.contentimpl.labellist.RapidFloatingActionContentLabelList;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Objects;

public class MainActivity extends AppCompatActivity implements RapidFloatingActionContentLabelList.OnRapidFloatingActionContentLabelListListener{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    public native void Jnimean(Bitmap bitmap);
    public native void Jnimedian(Bitmap bitmap);

    public static final int CAMERA_REQUEST_CODE = 0;
    public static final int GALLERY_REQUEST_CODE = 1;
    private static final int CAMERA_PERMISSION_CODE = 100;
    private static final int STORAGE_PERMISSION_CODE = 101;
    private RapidFloatingActionButton rfaBtn;
    private RapidFloatingActionHelper rfabHelper;
    private RapidFloatingActionLayout rfaLayout;
    private static final String TAG = "HomeFragment";
    private String currentPhotoPath;
    private ImageView imageView;
    private Button mean, median, save;
    Bitmap bitmap1, bitmap2;
    File imgFile;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        rfaBtn = findViewById(R.id.activity_main_rfab);
        rfaLayout = findViewById(R.id.activity_main_rfal);
        imageView = findViewById(R.id.imageView);
        mean = findViewById(R.id.mean_filter);
        median = findViewById(R.id.median_filter);
        save = findViewById(R.id.save);


        initFab();

        // Example of a call to a native method
//        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());

        mean.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                bitmap1 = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
                Thread t = new Thread(){
                    public void run(){
                        bitmap2 = bitmap1.copy(Bitmap.Config.ARGB_8888, true);
                        Jnimean(bitmap2);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                save.setVisibility(View.VISIBLE);
                                imageView.setImageBitmap(bitmap2);

                            }
                        });

                    }
                };
                t.start();

            }
        });

        median.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                bitmap1 = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
                Thread t = new Thread(){
                    public void run(){
                        bitmap2 = bitmap1.copy(Bitmap.Config.ARGB_8888, true);
                        Jnimean(bitmap2);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                save.setVisibility(View.VISIBLE);
                                imageView.setImageBitmap(bitmap2);

                            }
                        });

                    }
                };
                t.start();
            }
        });
    }

    private void initFab() {
        RapidFloatingActionContentLabelList rfaContent = new RapidFloatingActionContentLabelList(this);
        rfaContent.setOnRapidFloatingActionContentLabelListListener(this);
        List<RFACLabelItem> items = new ArrayList<>();

        items.add(new RFACLabelItem<Integer>()
                .setLabel("Camera")
                .setResId(R.drawable.ic_camera)
                .setIconNormalColor(ContextCompat.getColor(this, R.color.blue))
        );
        items.add(new RFACLabelItem<Integer>()
                .setLabel("Gallery")
                .setResId(R.drawable.ic_gallery)
                .setIconNormalColor(ContextCompat.getColor(this, R.color.blue))
        );


        rfaContent
                .setItems(items)
                .setIconShadowColor(0xff888888);

        rfabHelper = new RapidFloatingActionHelper(
                this,
                rfaLayout,
                rfaBtn,
                rfaContent
        ).build();
    }

    @Override
    public void onRFACItemLabelClick(int position, RFACLabelItem item) {
        intentAction(position);
    }

    @Override
    public void onRFACItemIconClick(int position, RFACLabelItem item) {
        intentAction(position);
    }


    private void askGalleryPermission() {
        if (ContextCompat.checkSelfPermission(Objects.requireNonNull(this), Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(Objects.requireNonNull(this), new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, STORAGE_PERMISSION_CODE);
        } else {
            Intent gallery = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
            startActivityForResult(gallery, GALLERY_REQUEST_CODE);
        }
    }

    private void dispatchTakePictureIntent() {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        // Ensure that there's a camera activity to handle the intent
        if (takePictureIntent.resolveActivity(Objects.requireNonNull(this).getPackageManager()) != null) {
            // Create the File where the photo should go
            File photoFile = null;
            Log.d("MAIN", "Inside dispatch method");
            try {
                photoFile = createImageFile();
            } catch (IOException ex) {
                Log.d("MAIN", Objects.requireNonNull(ex.getMessage()));

            }
            // Continue only if the File was successfully created
            if (photoFile != null) {
                Log.d("MAIN", "File Successfully created");

                Uri photoURI = FileProvider.getUriForFile(Objects.requireNonNull(MainActivity.this),
                        "imageediting.example.android.fileprovider",
                        photoFile);
                takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
                startActivityForResult(takePictureIntent, CAMERA_REQUEST_CODE);
            }
        }
    }

    private File createImageFile() throws IOException {
        // Create an image file name
        Log.d("MAIN", "INSide createImage method");

        @SuppressLint("SimpleDateFormat") String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "JPEG_" + timeStamp + "_";
        File storageDir = Objects.requireNonNull(this).getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        // File storageDir = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES);
        Log.d("MAIN", "After" + Objects.requireNonNull(storageDir).toString());

        File image = File.createTempFile(
                imageFileName,  /* prefix */
                ".jpg",         /* suffix */
                storageDir      /* directory */
        );
        Log.d("MAIN", image.toString());
        // Save a file: path for use with ACTION_VIEW intents
        currentPhotoPath = image.getAbsolutePath();
        return image;
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case CAMERA_REQUEST_CODE:
                if (resultCode == RESULT_OK) {
                    File f = new File(currentPhotoPath);
                    Uri contentUri = Uri.fromFile(f);
                    imgFile = new File(contentUri.getPath());
//                    Intent intent = new Intent(this, SaveDetails.class);
//                    intent.putExtra("uri_image", contentUri);
//                    intent.putExtra("file_name", f.getName());
//                    startActivity(intent);

                    Glide.with(MainActivity.this).load(contentUri)
                            .into(imageView);

                }
                break;
            case GALLERY_REQUEST_CODE:
                if (resultCode == RESULT_OK && data != null) {
                    Uri contentUri = data.getData();
                    String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
                    String imageFileName = "JPEG_" + timeStamp + "." + getFileExt(contentUri);
                    imgFile = new File(contentUri.getPath());
                    Glide.with(MainActivity.this).load(contentUri)
                            .into(imageView);
                }
                break;
        }
    }

    private String getFileExt(Uri contentUri) {
        ContentResolver c = getContentResolver();
        MimeTypeMap mime = MimeTypeMap.getSingleton();
        return mime.getExtensionFromMimeType(c.getType(contentUri));
    }


    private void askCameraPermissions() {
        if (ContextCompat.checkSelfPermission(Objects.requireNonNull(this), Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(Objects.requireNonNull(this), new String[]{Manifest.permission.CAMERA}, CAMERA_PERMISSION_CODE);
        } else {
            dispatchTakePictureIntent();
        }

    }

    private void intentAction(int position) {
        if (position == 0) askCameraPermissions();
        if (position == 1) askGalleryPermission();
        rfabHelper.toggleContent();
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    public native String stringFromJNI();
}
