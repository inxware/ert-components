Shader "Custom/MaintainImageTextureRatio"
{
    Properties
    {
        [MainColor] _Color ("Color", Color) = (1,1,1,1)
        _BackgroundColor ("BackgroundColor", Color) = (1,1,1,1)
        [MainTexture] _MainTex ("Albedo", 2D) = "white" {}
        _Glossiness ("Smoothness", Range(0,1)) = 0.5
        _Metallic ("Metallic", Range(0,1)) = 0.0
    }
    SubShader
    {
        Tags {  "Queue" = "Transparent" "IgnoreProjector" = "True" "RenderType" = "Transparent" }
        LOD 100

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface surf Standard fullforwardshadows alpha:fade

        // Use shader model 3.0 target, to get nicer looking lighting
        #pragma target 3.0

        sampler2D _MainTex;

        fixed _RectMinX;
        fixed _RectMaxX;
        fixed _RectMinY;
        fixed _RectMaxY;

        struct Input
        {
            float2 uv_MainTex;
        };

        half _Glossiness;
        half _Metallic;
        fixed4 _Color;
        fixed4 _BackgroundColor;

        // Add instancing support for this shader. You need to check 'Enable Instancing' on materials that use the shader.
        // See https://docs.unity3d.com/Manual/GPUInstancing.html for more information about instancing.
        // #pragma instancing_options assumeuniformscaling
        UNITY_INSTANCING_BUFFER_START(Props)
            // put more per-instance properties here
        UNITY_INSTANCING_BUFFER_END(Props)

        void surf (Input IN, inout SurfaceOutputStandard o)
        {
            _RectMinX = 0;
            _RectMinY = 0;
            _RectMaxX = 1;
            _RectMaxY = 1;

            //preRect x and y each represent 1 dimensional min and max ranges for rectangle. When they are multiplied together, they form a white rectangle mask (where they intersect).
            float2 preRect;
            preRect.x = (IN.uv_MainTex.x > _RectMinX) - (IN.uv_MainTex.x > _RectMaxX);
            preRect.y = (IN.uv_MainTex.y > _RectMinY) - (IN.uv_MainTex.y > _RectMaxY);
            half rectMask = preRect.x * preRect.y;

            //uv_OffsetCoord.x and y copy the uv coordinates of the main texture and are offsetted.
            //Then, the old uv coordinates are blended with the new uv coordinates, using the rectangle as a mask.
            float2 uv_OffsetCoord = IN.uv_MainTex;

            //So now, the problem is, offsetting will cause the UV values will go lower than 0 or higher than 1.
            //Well, fortunately, we can use frac() to continuously repeat the texture (between 0 and 1) forever!
            uv_OffsetCoord.x = frac(uv_OffsetCoord.x);
            uv_OffsetCoord.y = frac(uv_OffsetCoord.y);

            //Blend old uv coordinates with new offsetted uv coordinates, using the rectangle as a mask
            IN.uv_MainTex = (IN.uv_MainTex * (1 - rectMask)) + (uv_OffsetCoord * rectMask);

            fixed4 c = tex2D (_MainTex, IN.uv_MainTex) * _Color;
            c.rgb = (c.rgb * rectMask) + ((1 - rectMask) * _BackgroundColor.rgb);
            c.a = (rectMask * c.a) + ((1 - rectMask) * _BackgroundColor.a);
            // Use emission so the texture is visible regardless of scene lighting direction.
            // Shape face normals can point away from any scene light, making Albedo-only
            // surfaces render black. Emission is unaffected by lighting.
            o.Albedo = float3(0, 0, 0);
            o.Emission = c.rgb;
            o.Alpha = c.a;
            o.Metallic = 0;
            o.Smoothness = 0;
        }
        ENDCG
    }
    FallBack "Diffuse"
}
