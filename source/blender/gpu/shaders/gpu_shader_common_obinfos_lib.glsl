
/* Need to be included after common_view_lib.glsl for resource_id. */
#ifndef GPU_OBINFOS_UBO
#define GPU_OBINFOS_UBO
struct ObjectInfos {
  vec4 drw_OrcoTexCoFactors[2];
  vec4 drw_Infos;
};

layout(std140) uniform infoBlock
{
  /* DRW_RESOURCE_CHUNK_LEN = 512 */
  ObjectInfos drw_infos[512];
};
#define OrcoTexCoFactors (drw_infos[resource_id].drw_OrcoTexCoFactors)
#define ObjectInfo (drw_infos[resource_id].drw_Infos)
#endif
