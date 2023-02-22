//
// 播放动画需求，根据animationList来控制glb节点的动画
//
// Created by panda on 2020-03-06.
//

#ifndef TNT_ANIMATIONCOMPONENT_H
#define TNT_ANIMATIONCOMPONENT_H

#include <gltfio/ModelDef.h>
#include <string>
#include <math/vec2.h>
LIGHT3D_NAMESPACE_BEGIN
class AnimationComponent {
 public:
    std::vector<std::string> triggerableAnimationNameList;
    std::vector<TriggerableAnimationItem> triggerableAnimationItemList;
    std::map<std::string, float> animationNameToDurationMap;

 public:
    /**
     * 节点注册
     * @param nameList 所有在json列配置的节点名列表
     */
    void registerAnimation(std::vector<std::string> &nameList);

    /**
     * 播放动画
     * @param renderTime 将上层传入的间隔时间累加起来，用于动画速率随帧率变化
     * @param name  node节点名
     * @param count 动画播放的次数，0表示无限循环
     */
    void playAnimation(float renderTime, const std::string& name, int count);

    /**
     * 动画是否在播放
     * @param name node节点名
     * @return
     */
    bool isAnimationRunning(const std::string &name);

    /**
     * 停止动画
     * @param name node节点名
     */
    void stopAnimation(const std::string& name);

    /**
     * 判断是不是被动态触发的节点
     *
     * @param name node节点名
     * @return
     */
    bool isTriggerableAnimation(const std::string& name);

    /**
     * 获取被触发节点的动画持续时间
     *
     * @param name node节点名
     * @return
     */
    float getAnimationDuration(const std::string& name);

    /**
     * 获取该节点动画的持续时间
     *
     * @param name node节点名
     * @return (startTime, endTime)
     */
    filament::math::float2 getAnimationTime(const std::string& name);
};
LIGHT3D_NAMESPACE_END
#endif //TNT_ANIMATIONCOMPONENT_H
