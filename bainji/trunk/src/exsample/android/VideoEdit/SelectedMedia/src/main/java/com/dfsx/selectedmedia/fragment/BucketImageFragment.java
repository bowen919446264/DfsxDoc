package com.dfsx.selectedmedia.fragment;

import java.util.ArrayList;

import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.ImageColumns;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;
import android.widget.Toast;
import com.dfsx.selectedmedia.BucketEntry;
import com.dfsx.selectedmedia.MediaChooserConstants;
import com.dfsx.selectedmedia.R;
import com.dfsx.selectedmedia.activity.HomeFragmentActivity;
import com.dfsx.selectedmedia.adapter.BucketGridAdapter;


public class BucketImageFragment extends Fragment{

	// The indices should match the following projections.
	private final int INDEX_BUCKET_ID     = 0;
	private final int INDEX_BUCKET_NAME   = 1;
	private final int INDEX_BUCKET_URL    = 2;

	private static final String[] PROJECTION_BUCKET = {
		ImageColumns.BUCKET_ID,
		ImageColumns.BUCKET_DISPLAY_NAME,
		ImageColumns.DATA};

	private View mView;
	private GridView mGridView;
	private BucketGridAdapter mBucketAdapter;

	private Cursor mCursor;



	public BucketImageFragment(){
		setRetainInstance(true);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		if(mView == null){
			mView     = inflater.inflate(R.layout.view_grid_layout_media_chooser, container, false);
			mGridView = (GridView)mView.findViewById(R.id.gridViewFromMediaChooser);
			init();
		}else{
			((ViewGroup) mView.getParent()).removeView(mView);
			if(mBucketAdapter == null){
				Toast.makeText(getActivity(), getActivity().getString(R.string.no_media_file_available), Toast.LENGTH_SHORT).show();
			}
		}
		return mView;
	}


	private void init(){
		final String orderBy = MediaStore.Images.Media.DATE_TAKEN;
		 mCursor = getActivity().getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, PROJECTION_BUCKET, null, null, orderBy + " DESC");
		ArrayList<BucketEntry> buffer = new ArrayList<BucketEntry>();
		try {
			while (mCursor.moveToNext()) {
				BucketEntry entry = new BucketEntry(
						mCursor.getInt(INDEX_BUCKET_ID),
						mCursor.getString(INDEX_BUCKET_NAME),mCursor.getString(INDEX_BUCKET_URL));

				if (! buffer.contains(entry)) {
					buffer.add(entry);
				}
			}

			if(mCursor.getCount() > 0){
				mBucketAdapter = new BucketGridAdapter(getActivity(), 0, buffer, false);
				mGridView.setAdapter(mBucketAdapter);
			}else{
				Toast.makeText(getActivity(), getActivity().getString(R.string.no_media_file_available), Toast.LENGTH_SHORT).show();
			}

			mGridView.setOnItemClickListener(new OnItemClickListener() {

				@Override
				public void onItemClick(AdapterView<?> adapter, View view,
						int position, long id) {

					BucketEntry bucketEntry  = (BucketEntry)adapter.getItemAtPosition(position);
					Intent selectImageIntent = new Intent(getActivity(),HomeFragmentActivity.class);
					selectImageIntent.putExtra("name", bucketEntry.bucketName);
					selectImageIntent.putExtra("image", true);
					selectImageIntent.putExtra("isFromBucket", true);
					getActivity().startActivityForResult(selectImageIntent, MediaChooserConstants.BUCKET_SELECT_IMAGE_CODE);
				}
			});

		} finally {
			mCursor.close();
		}
	}

	public BucketGridAdapter getAdapter() {
		return	mBucketAdapter;
	}


}
