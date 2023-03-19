#pragma once
#include"Base.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"ffAssimpHelper.hpp"
#include"ffBone.h"

namespace FF 
{
	struct ffAssimpNodeData
	{
		glm::mat4						m_transform;
		std::string						m_name;
		std::vector<ffAssimpNodeData>	m_children;

		ffAssimpNodeData() :m_transform(1.0f), m_name("") { m_children.clear(); }
	};

	typedef std::map<std::string, ffBoneInfo> ffBoneInfoMap;
	class ffAnimation
	{
	public:
		ffAnimation() = default;
		ffAnimation(const aiAnimation* _aiAnim, aiNode* _root, ffBoneInfoMap& _boneInfoMap, uint& _boneCount);

		inline float getTicksPerSecond() { return m_ticksPerSecond; }
		inline float getDuration() { return m_duration; }
		inline const ffAssimpNodeData& getRootNode() { return m_rootNode; }

		ffBone* findBone(std::string _name);
	private:
		void readAnimationData(const aiAnimation* _aiAnim, ffBoneInfoMap& _boneInfoMap, uint& _boneCount);
		void readHeirarchyData(ffAssimpNodeData& _dst, const aiNode* _src);
	private:
		float								m_duration;
		float								m_ticksPerSecond;
		std::vector<ffBone>					m_boneArr;
		ffBoneInfoMap						m_boneInfoMap;
		ffAssimpNodeData					m_rootNode;
	};
}


