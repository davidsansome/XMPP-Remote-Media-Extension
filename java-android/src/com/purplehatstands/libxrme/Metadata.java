package com.purplehatstands.libxrme;

import org.xmlpull.v1.XmlPullParser;

public class Metadata {
  public String title = "";
  public String artist = "";
  public String album = "";
  public String albumartist = "";
  public String composer = "";
  public String genre = "";
  public int track = 0;
  public int disc = 0;
  public int year = 0;
  public int length_millisec = 0;
  public double rating = 0.0; // range 0.0 - 1.0
  
  public static Metadata parse(XmlPullParser parser) throws Exception {
    Metadata ret = new Metadata();
    
    int count = parser.getAttributeCount();
    for (int i=0 ; i<count ; ++i) {
      String name = parser.getAttributeName(i);
      String value = parser.getAttributeValue(i);
      
      if (name.equals("title")) {
        ret.title = value;
      } else if (name.equals("artist")) {
        ret.artist = value;
      } else if (name.equals("album")) {
        ret.album = value;
      } else if (name.equals("albumartist")) {
        ret.albumartist = value;
      } else if (name.equals("composer")) {
        ret.composer = value;
      } else if (name.equals("genre")) {
        ret.genre = value;
      } else if (name.equals("track")) {
        ret.track = Integer.parseInt(value);
      } else if (name.equals("disc")) {
        ret.disc = Integer.parseInt(value);
      } else if (name.equals("year")) {
        ret.year = Integer.parseInt(value);
      } else if (name.equals("length_millisec")) {
        ret.length_millisec = Integer.parseInt(value);
      } else if (name.equals("rating")) {
        ret.rating = Double.parseDouble(value);
      }
    }
    
    while (true) {
      int event_type = parser.next();
      if (event_type == XmlPullParser.END_TAG) {
        if (parser.getName().equals("metadata")) {
          break;
        }
      }
    }
    
    return ret;
  }
}
