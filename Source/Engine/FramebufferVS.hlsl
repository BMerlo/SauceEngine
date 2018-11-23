/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/
struct VOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

VOut FramebufferVS( float4 position : POSITION,float2 texCoord : TEXCOORD0 )
{
	VOut output;

	output.position = position;
	output.texCoord = texCoord;

	return output;
}