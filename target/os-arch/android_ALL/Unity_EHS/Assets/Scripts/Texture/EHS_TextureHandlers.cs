using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class TextureHandler
{
    protected EHS_MediaSource mediaSource;

    public TextureHandler(EHS_MediaSource mediaSource)
    {
        this.mediaSource = mediaSource;
    }

    public bool Update(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        if(config != null){
            var textureContainer = ehsObject.GetTextureContainer();
            return Update(ehsObject, config, textureContainer.Get(config.path));
        }
        return false;
    }

    public bool Update(EHS_Object ehsObject, EHS_MediaSourceConfig config, Texture texture)
    {
        string[] sides = EHS_ObjectUtils.GetSides(config.sides);
        return Update(ehsObject, texture, sides);
    }

    public abstract bool Update(EHS_Object ehsObject, Texture texture, string[] sides);
}

public class EHS_TextureHandlers
{
    protected EHS_MediaSource mediaSource;
    private Dictionary<string, TextureHandler> textureHandlers;

    public EHS_TextureHandlers(EHS_MediaSource mediaSource) 
    {
        this.mediaSource = mediaSource;

        textureHandlers = new Dictionary<string, TextureHandler>();
        // register texture handlers
        // (1) Basic handler
        textureHandlers.Add(EHS_BasicTextureHandler.HandlerType, new EHS_BasicTextureHandler(this.mediaSource));
        // (2) Quad Cube handler
        textureHandlers.Add(EHS_QuadObjectSurfaceHandler.QuadObjectTextureHandler.HandlerType, 
                            new EHS_QuadObjectSurfaceHandler.QuadObjectTextureHandler(this.mediaSource));
        // (3) Quad handler
        textureHandlers.Add(EHS_QuadHandler.QuadTextureHandler.HandlerType, 
                            new EHS_QuadHandler.QuadTextureHandler(this.mediaSource));
        // (4) 3x3 Matrix handler // @TODO - fix later
        //textureHandlers.Add(EHS_MatrixHandler.MatrixTextureHandler.HandlerType, 
        //                    new EHS_MatrixHandler.MatrixTextureHandler(this.mediaSource));
        // ...
    }

    public bool Update(EHS_Object ehsObject, EHS_MediaSourceConfig config)
    {
        if(config != null && textureHandlers.ContainsKey(config.handler)){
            var handler = textureHandlers[config.handler];
            return handler.Update(ehsObject, config);
        } 
        return false;
    }

    public bool Update(EHS_Object ehsObject, EHS_MediaSourceConfig config, Texture texture)
    {
        if(config != null && textureHandlers.ContainsKey(config.handler)){
            var handler = textureHandlers[config.handler];
            return handler.Update(ehsObject, config, texture);
        } 
        return false;
    }
}
