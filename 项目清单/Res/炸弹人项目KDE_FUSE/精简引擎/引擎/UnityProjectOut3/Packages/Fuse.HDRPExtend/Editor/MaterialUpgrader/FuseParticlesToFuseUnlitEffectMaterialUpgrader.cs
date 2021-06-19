//===========================================================================
//!
//!	@file		FuseParticlesToFuseUnlitEffectMaterialUpgrader.cs
//!	@brief		G4�p�[�e�B�N���}�e���A������G5Unlit�G�t�F�N�g�}�e���A���̃A�b�v�O���[�_�[
//!
//!	@author		Copyright (C) 2020 HEXADRIVE Inc. All rights reserved.
//!	@author		K.Fujihira
//!
//===========================================================================
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.HighDefinition;

using static UnityEngine.Rendering.HighDefinition.HDMaterialProperties;

namespace UnityEditor.Rendering.HighDefinition
{
	//===========================================================================
	//!	@class		FuseParticlesToFuseUnlitEffectMaterialUpgrader
	//!	@brief		G4�p�[�e�B�N���}�e���A������G5Unlit�G�t�F�N�g�}�e���A���̃A�b�v�O���[�_�[
	//===========================================================================
	public class FuseParticlesToFuseUnlitEffectMaterialUpgrader : MaterialUpgrader
	{
		//-----------------------------------------------------------
		//!	@name �萔
		//-----------------------------------------------------------
		//@{

		private const int	RENDER_QUEUE_STEP	= 5;	//!< �����_�[�L���[������傫��

		//@}


		//-----------------------------------------------------------
		//!	@name ���J���\�b�h
		//-----------------------------------------------------------
		//@{

		//-----------------------------------------------------------
		//!	@brief	�R���X�g���N�^
		//!	@param	[in]	sourceShaderName	���̃V�F�[�_�[��
		//!	@param	[in]	destShaderName		�o�͂̃V�F�[�_�[��
		//!	@param	[in]	finalizer			�t�@�C�i���C�U
		//-----------------------------------------------------------
		public FuseParticlesToFuseUnlitEffectMaterialUpgrader(string sourceShaderName, string destShaderName, MaterialFinalizer finalizer = null)
		{
			RenameShader(sourceShaderName, destShaderName, finalizer);

			RenameColor("_TintColor",			"_TintColor_G5");
			RenameTexture("_MainTex",			"_MainTexture_G5");
			RenameColor("_MainTex_ST",			"_MainTextureTilingOffset_G5");
			RenameFloat("_InvFade",				"_SoftParticleFactor_G5");
			RenameFloat("_ColorStrength",		"_ColorStrength_G5");
			RenameTexture("_DissolveMap",		"_DissolveMap_G5");
			RenameColor("_DissolveMap_ST",		"_DissolveMapTilingOffset_G5");
			RenameFloat("_GradientRange",		"_GradientRange_G5");
			RenameFloat("_DissolveThreshold",	"_DissolveThreshold_G5");
		}

		//-----------------------------------------------------------
		//!	@brief	�R���o�[�g
		//!	@param	[in]	srcMaterial	���}�e���A��
		//!	@param	[in]	dstMaterial	�o�̓}�e���A��
		//!	@return	�Ȃ�
		//-----------------------------------------------------------
		public override void Convert(Material srcMaterial, Material dstMaterial)
		{
			if( srcMaterial == null ||
				dstMaterial == null ) return;

			dstMaterial.hideFlags = HideFlags.DontUnloadUnusedAsset;

			base.Convert(srcMaterial, dstMaterial);

			// �����_���[�L���[�ϊ�
			{
				const int TRANSPARENT_QUEUE = (int)RenderQueue.Transparent;
				const int RANGE = HDRenderQueue.Priority.TransparentLast - HDRenderQueue.Priority.TransparentFirst;
				const int RANGE_HALF = RANGE / 2;

				var renderQueue = srcMaterial.renderQueue;
				var priority = renderQueue - TRANSPARENT_QUEUE;
				var newPriority = 0;

				string getPriorityRangeText() { return $"-{RANGE_HALF}�`{RANGE_HALF}"; }
				// �����_�[�L���[�̌x�����O�o��
				void warningRenderQueueLog(string text)
				{
					Debug.LogWarning($"renderQueue����SortingPriority({getPriorityRangeText()})�̕ϊ��Ō덷���������Ă��܂��B\n{text}\n�}�e���A���� : {dstMaterial.name}\n�ϊ�����renderQueue : {renderQueue} �� {newPriority}\n\n");
				}

				if( priority < -RANGE_HALF ) {
					newPriority = -RANGE_HALF;
					warningRenderQueueLog($"����renderQueue��{TRANSPARENT_QUEUE + -RENDER_QUEUE_STEP}��������Ă���ׁA{getPriorityRangeText()}�ɕϊ��ł��܂���ł����B");
				}
				if( priority <= 0 ) {
					newPriority = priority;
				}
				else {
					var floatPriority = priority / (float)RENDER_QUEUE_STEP;
					newPriority = Mathf.CeilToInt(floatPriority);
					if( newPriority != floatPriority ) {
						warningRenderQueueLog($"����renderQueue��{RENDER_QUEUE_STEP}�̔{���łȂ��ׁA�J��グ���������܂���");
					}
					if( newPriority > RANGE_HALF ) {
						newPriority = RANGE_HALF;
						warningRenderQueueLog($"����renderQueue��{TRANSPARENT_QUEUE + RENDER_QUEUE_STEP * RANGE_HALF}�������Ă���ׁA{getPriorityRangeText()}�ɕϊ��ł��܂���ł����B");
					}
				}
				dstMaterial.SetInt(kTransparentSortPriority, newPriority);
			}

			if( !HDShaderUtils.ResetMaterialKeywords(dstMaterial) ) {
				CoreEditorUtils.RemoveMaterialKeywords(dstMaterial);
				// We need to reapply ToggleOff/Toggle keyword after reset via ApplyMaterialPropertyDrawers
				MaterialEditor.ApplyMaterialPropertyDrawers(dstMaterial);
				FuseHDUnlitEffectGUI.SetupMaterialKeywordsAndPass(dstMaterial);
				EditorUtility.SetDirty(dstMaterial);
			}
		}

		//@}
	}
}
//===========================================================================
//	END OF FILE
//===========================================================================
