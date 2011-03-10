package com.purplehatstands.libxrme;

import org.jivesoftware.smack.packet.IQ;
import org.jivesoftware.smack.provider.IQProvider;
import org.xmlpull.v1.XmlPullParser;

public class State extends IQ {
  public static final int PLAYBACKSTATE_STOPPED = 0;
  public static final int PLAYBACKSTATE_PLAYING = 1;
  public static final int PLAYBACKSTATE_PAUSED = 2;
  
  public int playback_state = PLAYBACKSTATE_STOPPED;
  public int position_millisec = 0;
  public double volume = 0.0;
  public boolean can_go_next = false;
  public boolean can_go_previous = false;
  public boolean can_seek = false;
  public int length_millisec = 0;
  public Metadata metadata = new Metadata();
  
  
  public static class Parser implements IQProvider {
    public IQ parseIQ(XmlPullParser parser) throws Exception {
      State ret = new State();
      
      int count = parser.getAttributeCount();
      for (int i=0 ; i<count ; ++i) {
        String name = parser.getAttributeName(i);
        String value = parser.getAttributeValue(i);
        
        if (name.equals("playback_state")) {
          ret.playback_state = Integer.parseInt(value);
        } else if (name.equals("position_millisec")) {
          ret.position_millisec = Integer.parseInt(value);
        } else if (name.equals("volume")) {
          ret.volume = Double.parseDouble(value);
        } else if (name.equals("can_go_next")) {
          ret.can_go_next = Integer.parseInt(value) == 0 ? false : true;
        } else if (name.equals("can_go_previous")) {
          ret.can_go_previous = Integer.parseInt(value) == 0 ? false : true;
        } else if (name.equals("can_seek")) {
          ret.can_seek = Integer.parseInt(value) == 0 ? false : true;
        } else if (name.equals("length_millisec")) {
          ret.length_millisec = Integer.parseInt(value);
        }
      }
      
      while (true) {
        int event_type = parser.next();
        if (event_type == XmlPullParser.START_TAG) {
          if (parser.getName().equals("metadata")) {
            ret.metadata = Metadata.parse(parser);
          }
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
