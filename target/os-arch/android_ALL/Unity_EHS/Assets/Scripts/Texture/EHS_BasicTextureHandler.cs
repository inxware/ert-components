using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EHS_BasicTextureHandler : TextureHandler
{
    public static readonly string HandlerType = "basic";

    public EHS_BasicTextureHandler(EHS_MediaSource mediaSource) : base(mediaSource)
    {
    }

    public override bool Update(EHS_Object ehsObject, Texture texture, string[] sides)
    {
        return EHS_TextureUtils.UpdateObjectTexture(ehsObject.GetGameObject(), texture);
    }
}
