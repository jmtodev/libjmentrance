package id.co.jmto.libentrancetest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import id.co.jmto.entrancelib.EntranceLib;

public class ExampleActivity extends AppCompatActivity {
  EntranceLib entranceLib;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    TextView txt = findViewById(R.id.txt);

    entranceLib = new EntranceLib();

    String sn = "7546000012122780";
    String data = "615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF";
    String buf = "TEST DATA ENTRANCE JJS\n\n"
        + "SN     = " + sn + "\n"
        + "INPUT  = " + data + "\n";
    try {
      String ret = entranceLib.jjsDecrypt(sn, data);
      buf += "OUTPUT = " + ret + "\n";
    } catch (Exception e) {
      buf += "OUTPUT = ERROR\n";
      throw new RuntimeException(e);
    }
    txt.setText(buf);
  }
}