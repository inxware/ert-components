using UnityEngine;

public class EHS_AnimationUtils
{
    // animation directions
    public static readonly string Left2Right = "left2right";
    public static readonly string Right2Left = "right2left";
    public static readonly string Top2Bottom = "top2bottom";
    public static readonly string Bottom2Top = "bottom2top";

    public static Vector3 Direction(string directionString){ 
        Vector3 direction = Vector3.zero;
        if(Left2Right.Equals(directionString)){ 
            return new Vector3(1f, 0f, 0f);    
        }
        if(Right2Left.Equals(directionString)){ 
            return new Vector3(-1f, 0f, 0f);    
        }
        if(Top2Bottom.Equals(directionString)){ 
            return new Vector3(0f, -1f, 0f);    
        }
        if(Bottom2Top.Equals(directionString)){ 
            return new Vector3(0f, 1f, 0f);    
        }
        return direction;
    }

    public static bool IsDirection(string directionString){ 
        return !string.IsNullOrEmpty(directionString) &&
            (Left2Right.Equals(directionString) || 
             Right2Left.Equals(directionString) || 
             Top2Bottom.Equals(directionString) || 
             Bottom2Top.Equals(directionString));
    }

    public static Vector3 OutOfScreenPosition(Vector3 direction, Rect bounds){
        var screen = EHS_Utils.FillScreen();
        float x = direction.x * (screen.x/2 - (direction.x*bounds.x) + bounds.width/2);
        float y = direction.y * (screen.y/2 - (direction.y*bounds.y) + bounds.height/2);
        return new Vector3(x, y, 0f);
    }

    public static string InvertDirection(string direction){ 
        if(Left2Right.Equals(direction)){ 
            return Right2Left;
        }else if(Right2Left.Equals(direction)){ 
            return Left2Right;
        }else if(Top2Bottom.Equals(direction)){ 
            return Bottom2Top;
        }else if(Bottom2Top.Equals(direction)){ 
            return Top2Bottom;
        }
        return null;    
    }
}
