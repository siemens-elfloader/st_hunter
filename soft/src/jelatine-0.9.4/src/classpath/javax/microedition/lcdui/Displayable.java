package javax.microedition.lcdui;
public abstract class Displayable
  extends Object
{
  private String title;
  private Ticker ticker;
  private boolean isShown;
  private int width;
  private int height;

  public String getTitle()
  {
    return title;
  }

  public void setTitle(String t)
  {
    title = t;
  }

  public Ticker getTicker()
  {
    return ticker;
  }

  public void setTicker(Ticker t)
  {
    ticker = t;
  }

  public boolean isShown()
  {
    return isShown;
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

  public int getWidth()
  {
    return width;
  }

  public int getHeight()
  {
    return height;
  }

  protected void sizeChanged(int w, int h)
  {
    width = w;
    height = h;
  }
}

