package javax.microedition.lcdui;
public class AlertType
  extends Object
{
  private static final int TYPE_INFO         = 1;
  private static final int TYPE_WARNING      = 2;
  private static final int TYPE_ERROR        = 3;
  private static final int TYPE_ALARM        = 4;
  private static final int TYPE_CONFIRMATION = 5;
  
  public static final AlertType INFO          = new AlertType(TYPE_INFO);
  public static final AlertType WARNING       = new AlertType(TYPE_WARNING);
  public static final AlertType ERROR         = new AlertType(TYPE_ERROR);
  public static final AlertType ALARM         = new AlertType(TYPE_ALARM);
  public static final AlertType CONFIRMATION  = new AlertType(TYPE_CONFIRMATION);

  private int type;

  protected AlertType()
  {
    type = TYPE_INFO;
  }

  protected AlertType(int id)
  {
    type = id;
  }

  public boolean playSound(Display d) throws NullPointerException
  {
    if (d == null)
      throw new NullPointerException();

    return true;
  }
}

