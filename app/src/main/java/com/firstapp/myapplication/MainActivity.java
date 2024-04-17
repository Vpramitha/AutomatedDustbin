package com.firstapp.myapplication;

        import android.os.Bundle;
        import android.util.Log;
        import android.widget.TextView;

        import androidx.annotation.NonNull;
        import androidx.appcompat.app.AppCompatActivity;

        import com.google.firebase.database.DataSnapshot;
        import com.google.firebase.database.DatabaseError;
        import com.google.firebase.database.DatabaseReference;
        import com.google.firebase.database.FirebaseDatabase;
        import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    private DatabaseReference mDatabase;

    String message="status";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView status= findViewById(R.id.status);

        // Initialize Firebase Database
        mDatabase = FirebaseDatabase.getInstance().getReference();

        // Read data from the database
        readData();
        status.setText(message.toString());
    }

    private void readData() {
        // Get a reference to the "data" node in the database
        DatabaseReference dataRef = mDatabase.child("data");

        // Add a listener for data changes
        dataRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                message = dataSnapshot.getValue(String.class);
                Log.d(TAG, "Data: " + message);

                // Update UI with the retrieved data
                TextView statusTextView = findViewById(R.id.status);
                statusTextView.setText(message);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                // Handle errors
                Log.w(TAG, "Failed to read value.", databaseError.toException());
            }
        });
    }
}

