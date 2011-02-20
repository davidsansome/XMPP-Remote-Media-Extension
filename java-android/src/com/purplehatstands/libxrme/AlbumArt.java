package com.purplehatstands.libxrme;

import org.jivesoftware.smack.packet.IQ;
import org.jivesoftware.smack.provider.IQProvider;
import org.xmlpull.v1.XmlPullParser;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;

public class AlbumArt extends IQ {
  Bitmap image;
  
  public static class Parser implements IQProvider {
    public IQ parseIQ(XmlPullParser parser) throws Exception {
      AlbumArt ret = new AlbumArt();
      
      while (true) {
        int event_type = parser.next();
        if (event_type == XmlPullParser.TEXT) {
          byte[] data = Base64.decode(parser.getText(), Base64.DEFAULT);
          ret.image = BitmapFactory.decodeByteArray(data, 0, data.length);
        } else if (event_type == XmlPullParser.END_TAG) {
          if (parser.getName().equals("state")) {
            break;
          }
        }
      }
      return ret;
    }
  }

  public String getChildElementXML() {
    return null;
  }

}
