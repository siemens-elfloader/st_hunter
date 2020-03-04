package javax.microedition.lcdui;
public class Alert
  extends Screen
{
  public static final int FOREVER = -2;
  // public static final Command DISMISS_COMMAND;

  private int timeout;
  private String title;
  private String text;
  private Image image;
  private AlertType type;
  private Gauge gauge;

  public Alert(String alertTitle)
  {
    title = alertTitle;
    text = null;
    image = null;
    type = null;
    timeout = FOREVER;
    gauge = null;
  }

  public Alert(String alertTitle, String alertText, Image alertImage, AlertType alertType)
  {
    title = alertTitle;
    text = alertText;
    image = alertImage;
    type = alertType;
    timeout = FOREVER;
    gauge = null;
  }

  public int getDefaultTimeout()
  {
    return FOREVER;
  }

  public int getTimeout()
  {
    return timeout;
  }

  public void setTimeout(int tmout) throws IllegalArgumentException
  {
    if((tmout <= 0) && (tmout != FOREVER))
      throw new IllegalArgumentException();
    else
      timeout = tmout; 
  }

  public AlertType getType()
  {
    return type;
  }

  public void setType(AlertType alertType)
  {
    type = alertType;
  }

  public String getString()
  {
    return text;
  }

  public void setString(String alertText)
  {
    text = alertText;
  }

  public Image getImage()
  {
    return image;
  }

  public void setImage(Image img)
  {
    image = img;
  }

  public Gauge getIndicator()
  {
    return gauge;
  }

  public void setIndicator(Gauge g) throws IllegalArgumentException
  {
    throw new IllegalArgumentException();
  }

  public void addCommand(Command cmd)
  {
  }

  public void removeCommand(Command cmd)
  {
  }

  public void setCommandListener(CommandListener l)
  {
  }

  public void showNotify()
  {
    showMsg(text);
  }

  private native void showMsg(String s);
}

