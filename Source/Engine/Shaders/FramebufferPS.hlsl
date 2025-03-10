/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/
Texture2D tex : register( t0 );
SamplerState sam : register( s0 );

float4 FramebufferPS( float4 position : SV_POSITION,float2 texCoord : TEXCOORD0 ) : SV_TARGET
{
	return tex.Sample( sam,texCoord );
}