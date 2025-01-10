#include <math.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <macros.h>

#include "__gx.h"

// GXLightObj private data
struct __GXLightObjInt_struct {
    u32 reserved[3];
    u32 Color;
    f32 a[3];
    f32 k[3];
    f32 lpos[3];
    f32 ldir[3];
};

void GXInitLightAttn(GXLightObj *lt_obj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x81, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x82, "GXInitLightAttn");
    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
    obj->k[0] = k0;
    obj->k[1] = k1;
    obj->k[2] = k2;
}

void GXInitLightAttnA(GXLightObj *lt_obj, f32 a0, f32 a1, f32 a2)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x8F, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x90, "GXInitLightAttnA");
    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
}

void GXGetLightAttnA(const GXLightObj *lt_obj, f32 *a0, f32 *a1, f32 *a2)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x99, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x9A, "GXGetLightAttnA");
    *a0 = obj->a[0];
    *a1 = obj->a[1];
    *a2 = obj->a[2];
}

void GXInitLightAttnK(GXLightObj *lt_obj, f32 k0, f32 k1, f32 k2)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0xA3, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0xA4, "GXInitLightAttnK");
    obj->k[0] = k0;
    obj->k[1] = k1;
    obj->k[2] = k2;
}

void GXGetLightAttnK(const GXLightObj *lt_obj, f32 *k0, f32 *k1, f32 *k2)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0xAD, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0xAE, "GXGetLightAttnK");
    *k0 = obj->k[0];
    *k1 = obj->k[1];
    *k2 = obj->k[2];
}

void GXInitLightSpot(GXLightObj *lt_obj, f32 cutoff, GXSpotFn spot_func)
{
    float a0, a1, a2;
    float r;
    float d;
    float cr;
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0xC6, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0xC8, "GXInitLightSpot");

    if (cutoff <= 0.0f || cutoff > 90.0f)
        spot_func = GX_SP_OFF;

    r = (3.1415927f * cutoff) / 180.0f;
    cr = cosf(r);
    switch (spot_func) {
    case GX_SP_FLAT:
        a0 = -1000.0f * cr;
        a1 = 1000.0f;
        a2 = 0.0f;
        break;
    case GX_SP_COS:
        a1 = 1.0f / (1.0f - cr);
        a0 = -cr * a1;
        a2 = 0.0f;
        break;
    case GX_SP_COS2:
        a2 = 1.0f / (1.0f - cr);
        a0 = 0.0f;
        a1 = -cr * a2;
        break;
    case GX_SP_SHARP:
        d = 1.0f / ((1.0f - cr) * (1.0f - cr));
        a0 = (cr * (cr - 2.0f)) * d;
        a1 = 2.0f * d;
        a2 = -d;
        break;
    case GX_SP_RING1:
        d = 1.0f / ((1.0f - cr) * (1.0f - cr));
        a2 = -4.0f * d;
        a0 = a2 * cr;
        a1 = (4.0f * (1.0f + cr)) * d;
        break;
    case GX_SP_RING2:
        d = 1.0f / ((1.0f - cr) * (1.0f - cr));
        a0 = 1.0f - ((2.0f * cr * cr) * d);
        a1 = (4.0f * cr) * d;
        a2 = -2.0f * d;
        break;
    case GX_SP_OFF:
    default:
        a0 = 1.0f;
        a1 = 0.0f;
        a2 = 0.0f;
        break;
    }
    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
}

void GXInitLightDistAttn(GXLightObj *lt_obj, f32 ref_dist, f32 ref_br, GXDistAttnFn dist_func)
{
    f32 k0, k1, k2;
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x111, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x113, "GXInitLightDistAttn");

    if (ref_dist < 0.0f)
        dist_func = GX_DA_OFF;
    if (ref_br <= 0.0f || ref_br >= 1.0f)
        dist_func = GX_DA_OFF;

    switch (dist_func) {
    case GX_DA_GENTLE:
        k0 = 1.0f;
        k1 = (1.0f - ref_br) / (ref_br * ref_dist);
        k2 = 0.0f;
        break;
    case GX_DA_MEDIUM:
        k0 = 1.0f;
        k1 = (0.5f * (1.0f - ref_br)) / (ref_br * ref_dist);
        k2 = (0.5f * (1.0f - ref_br)) / (ref_br * ref_dist * ref_dist);
        break;
    case GX_DA_STEEP:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = (1.0f - ref_br) / (ref_br * ref_dist * ref_dist);
        break;
    case GX_DA_OFF:
    default:
        k0 = 1.0f;
        k1 = 0.0f;
        k2 = 0.0f;
        break;
    }

    obj->k[0] = k0;
    obj->k[1] = k1;
    obj->k[2] = k2;
}

void GXInitLightPos(GXLightObj *lt_obj, f32 x, f32 y, f32 z)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x148, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x14A, "GXInitLightPos");

    obj->lpos[0] = x;
    obj->lpos[1] = y;
    obj->lpos[2] = z;
}

void GXGetLightPos(const GXLightObj *lt_obj, f32 *x, f32 *y, f32 *z)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x153, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x155, "GXGetLightPos");

    *x = obj->lpos[0];
    *y = obj->lpos[1];
    *z = obj->lpos[2];
}

void GXInitLightDir(GXLightObj *lt_obj, f32 nx, f32 ny, f32 nz)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x168, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;

    obj->ldir[0] = -nx;
    obj->ldir[1] = -ny;
    obj->ldir[2] = -nz;
}

void GXGetLightDir(const GXLightObj *lt_obj, f32 *nx, f32 *ny, f32 *nz)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x174, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;

    *nx = -obj->ldir[0];
    *ny = -obj->ldir[1];
    *nz = -obj->ldir[2];
}

void GXInitSpecularDir(GXLightObj *lt_obj, f32 nx, f32 ny, f32 nz)
{
    float mag;
    float vx;
    float vy;
    float vz;
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x18E, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x18F, "GXInitSpecularDir");

    vx = -nx;
    vy = -ny;
    vz = -nz + 1.0f;

    mag = (vx * vx) + (vy * vy) + (vz * vz);
    if (mag != 0.0f) {
        mag = 1.0f / sqrtf(mag);
    }
    
    obj->ldir[0] = vx * mag;
    obj->ldir[1] = vy * mag;
    obj->ldir[2] = vz * mag;
    obj->lpos[0] = nx * -1000000000000000000.0f;
    obj->lpos[1] = ny * -1000000000000000000.0f;
    obj->lpos[2] = nz * -1000000000000000000.0f;
}

void GXInitSpecularDirHA(GXLightObj *lt_obj, f32 nx, f32 ny, f32 nz, f32 hx, f32 hy, f32 hz)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x1B4, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x1B5, "GXInitSpecularHA");

    obj->ldir[0] = hx;
    obj->ldir[1] = hy;
    obj->ldir[2] = hz;
    obj->lpos[0] = nx * -1000000000000000000.0f;
    obj->lpos[1] = ny * -1000000000000000000.0f;
    obj->lpos[2] = nz * -1000000000000000000.0f;
}

void GXInitLightColor(GXLightObj *lt_obj, GXColor color)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x1CE, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x1CF, "GXInitLightColor");

    *(u32*)&obj->Color = *(u32*)&color;
}

void GXGetLightColor(const GXLightObj *lt_obj, GXColor *color)
{
    struct __GXLightObjInt_struct *obj;

    ASSERTMSGLINE(0x1DC, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x1DD, "GXGetLightColor");

    *(u32*)color = *(u32*)&obj->Color;
}

#if DEBUG
#define WRITE_SOME_LIGHT_REG1(val, addr) \
do {  \
    u32 xfData = val; \
    GX_WRITE_U32(val); \
    VERIF_MTXLIGHT(addr, xfData); \
} while (0)

#define WRITE_SOME_LIGHT_REG2(val, addr) \
do {  \
    f32 xfData = val; \
    GX_WRITE_F32(val); \
    VERIF_MTXLIGHT(addr, *(u32 *)&xfData); \
} while (0)
#else
#define WRITE_SOME_LIGHT_REG1(val, addr) GX_WRITE_U32(val)
#define WRITE_SOME_LIGHT_REG2(val, addr) GX_WRITE_F32(val)
#endif

static inline u32 ConvLightID2Num(GXLightID id) {
    switch (id) {
    case GX_LIGHT0: return 0;
    case GX_LIGHT1: return 1;
    case GX_LIGHT2: return 2;
    case GX_LIGHT3: return 3;
    case GX_LIGHT4: return 4;
    case GX_LIGHT5: return 5;
    case GX_LIGHT6: return 6;
    case GX_LIGHT7: return 7;
    default:        return 8;
    }
}

static inline void PushLight(const register GXLightObj* lt_obj, register void* dest) {
    register u32 zero, color;
    register f32 a0_a1, a2_k0, k1_k2;
    register f32 px_py, pz_dx, dy_dz;
#ifdef __MWERKS__  // clang-format off
	asm {
		lwz     color, 12(lt_obj)
		xor     zero, zero, zero
		psq_l   a0_a1, 16(lt_obj), 0, 0
		psq_l   a2_k0, 24(lt_obj), 0, 0
		psq_l   k1_k2, 32(lt_obj), 0, 0
		psq_l   px_py, 40(lt_obj), 0, 0
		psq_l   pz_dx, 48(lt_obj), 0, 0
		psq_l   dy_dz, 56(lt_obj), 0, 0

		stw     zero,  0(dest)
		stw     zero,  0(dest)
		stw     zero,  0(dest)
		stw     color, 0(dest)
		psq_st  a0_a1, 0(dest), 0, 0
		psq_st  a2_k0, 0(dest), 0, 0
		psq_st  k1_k2, 0(dest), 0, 0
		psq_st  px_py, 0(dest), 0, 0
		psq_st  pz_dx, 0(dest), 0, 0
		psq_st  dy_dz, 0(dest), 0, 0
	}
#endif  // clang-format on
}

void GXLoadLightObjImm(const GXLightObj *lt_obj, GXLightID light)
{
    u32 addr;
    u32 idx;
    struct __GXLightObjInt_struct * obj;

    ASSERTMSGLINE(0x238, lt_obj != NULL, "Light Object Pointer is null");
    obj = (struct __GXLightObjInt_struct *)lt_obj;
    CHECK_GXBEGIN(0x239, "GXLoadLightObjImm");

#if DEBUG
    idx = ConvLightID2Num(light);
#else
    idx = 31 - __cntlzw(light);
#endif

    ASSERTMSGLINE(0x23F, idx < 8, "GXLoadLightObjImm: Invalid Light Id");
    idx &= 7;

    addr = idx * 0x10 + 0x600;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(addr | 0xF0000);

#if DEBUG
    WRITE_SOME_LIGHT_REG1(0, addr);
    WRITE_SOME_LIGHT_REG1(0, addr + 1);
    WRITE_SOME_LIGHT_REG1(0, addr + 2);
    WRITE_SOME_LIGHT_REG1(obj->Color, addr + 3);
    WRITE_SOME_LIGHT_REG2(obj->a[0], addr + 4);
    WRITE_SOME_LIGHT_REG2(obj->a[1], addr + 5);
    WRITE_SOME_LIGHT_REG2(obj->a[2], addr + 6);
    WRITE_SOME_LIGHT_REG2(obj->k[0], addr + 7);
    WRITE_SOME_LIGHT_REG2(obj->k[1], addr + 8);
    WRITE_SOME_LIGHT_REG2(obj->k[2], addr + 9);
    WRITE_SOME_LIGHT_REG2(obj->lpos[0], addr + 10);
    WRITE_SOME_LIGHT_REG2(obj->lpos[1], addr + 11);
    WRITE_SOME_LIGHT_REG2(obj->lpos[2], addr + 12);
    WRITE_SOME_LIGHT_REG2(obj->ldir[0], addr + 13);
    WRITE_SOME_LIGHT_REG2(obj->ldir[1], addr + 14);
    WRITE_SOME_LIGHT_REG2(obj->ldir[2], addr + 15);
#else
    PushLight(lt_obj, (void*)GXFIFO_ADDR);
#endif

    __GXData->bpSentNot = 1;
}

void GXLoadLightObjIndx(u32 lt_obj_indx, GXLightID light)
{
    u32 reg;
    u32 addr;
    u32 idx;

    CHECK_GXBEGIN(0x270, "GXLoadLightObjIndx");

#if DEBUG
    idx = ConvLightID2Num(light);
#else
    idx = 31 - __cntlzw(light);
#endif

    ASSERTMSGLINE(0x273, idx < 8, "GXLoadLightObjIndx: Invalid Light Id");
    idx &= 7;

    addr = idx * 0x10 + 0x600;
    reg = 0;
    SET_REG_FIELD(0x278, reg, 12, 0, addr);
    SET_REG_FIELD(0x27A, reg, 4, 12, 0xF);
    SET_REG_FIELD(0x27A, reg, 16, 16, lt_obj_indx);
    GX_WRITE_U8(0x38);
    GX_WRITE_U32(reg);
#if DEBUG
    __GXShadowIndexState(7, reg);
#endif
    __GXData->bpSentNot = 1;
}

#define GXCOLOR_AS_U32(color) (*((u32*)&(color)))

void GXSetChanAmbColor(GXChannelID chan, GXColor amb_color)
{
    u32 reg;
    u32 rgb;
    u32 colIdx;

    CHECK_GXBEGIN(0x295, "GXSetChanAmbColor");

    switch (chan) {
    case GX_COLOR0:
        reg = __GXData->ambColor[GX_COLOR0];
        rgb = GXCOLOR_AS_U32(amb_color) >> 8;
        SET_REG_FIELD(0x2A3, reg, 24, 8, rgb);
        colIdx = 0;
        break;
    case GX_COLOR1:
        reg = __GXData->ambColor[GX_COLOR1];
        rgb = GXCOLOR_AS_U32(amb_color) >> 8;
        SET_REG_FIELD(0x2B2, reg, 24, 8, rgb);
        colIdx = 1;
        break;
    case GX_ALPHA0:
        reg = __GXData->ambColor[GX_COLOR0];
        SET_REG_FIELD(0x2B8, reg, 8, 0, amb_color.a);
        colIdx = 0;
        break;
    case GX_ALPHA1:
        reg = __GXData->ambColor[GX_COLOR1];
        SET_REG_FIELD(0x2BE, reg, 8, 0, amb_color.a);
        colIdx = 1;
        break;
    case GX_COLOR0A0:
        reg = GXCOLOR_AS_U32(amb_color);
        colIdx = 0;
        break;
    case GX_COLOR1A1:
        reg = GXCOLOR_AS_U32(amb_color);
        colIdx = 1;
        break;
    default:
        ASSERTMSGLINE(0x2DB, 0, "GXSetChanAmbColor: Invalid Channel Id");
        return;
    }

    GX_WRITE_XF_REG(colIdx + 10, reg);
    __GXData->bpSentNot = 1;
    __GXData->ambColor[colIdx] = reg;
}

void GXSetChanMatColor(GXChannelID chan, GXColor mat_color)
{
    u32 reg;
    u32 rgb;
    u32 colIdx;

    CHECK_GXBEGIN(0x2FA, "GXSetChanMatColor");

    switch (chan) {
    case GX_COLOR0:
        reg = __GXData->matColor[GX_COLOR0];
        rgb = GXCOLOR_AS_U32(mat_color) >> 8;
        SET_REG_FIELD(0x308, reg, 24, 8, rgb);
        colIdx = 0;
        break;
    case GX_COLOR1:
        reg = __GXData->matColor[GX_COLOR1];
        rgb = GXCOLOR_AS_U32(mat_color) >> 8;
        SET_REG_FIELD(0x317, reg, 24, 8, rgb);
        colIdx = 1;
        break;
    case GX_ALPHA0:
        reg = __GXData->matColor[GX_COLOR0];
        SET_REG_FIELD(0x31D, reg, 8, 0, mat_color.a);
        colIdx = 0;
        break;
    case GX_ALPHA1:
        reg = __GXData->matColor[GX_COLOR1];
        SET_REG_FIELD(0x323, reg, 8, 0, mat_color.a);
        colIdx = 1;
        break;
    case GX_COLOR0A0:
        reg = GXCOLOR_AS_U32(mat_color);
        colIdx = 0;
        break;
    case GX_COLOR1A1:
        reg = GXCOLOR_AS_U32(mat_color);
        colIdx = 1;
        break;
    default:
        ASSERTMSGLINE(0x340, 0, "GXSetChanMatColor: Invalid Channel Id");
        return;
    }

    GX_WRITE_XF_REG(colIdx + 12, reg);
    __GXData->bpSentNot = 1;
    __GXData->matColor[colIdx] = reg;
}

void GXSetNumChans(u8 nChans)
{
    CHECK_GXBEGIN(0x359, "GXSetNumChans");
    ASSERTMSGLINE(0x35A, nChans <= 2, "GXSetNumChans: nChans > 2");

    SET_REG_FIELD(0x35C, __GXData->genMode, 3, 4, nChans);
    GX_WRITE_XF_REG(9, nChans);
    __GXData->dirtyState |= 4;
}

void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src,
    GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn)
{
    u32 reg;
    u32 idx;

    CHECK_GXBEGIN(0x37C, "GXSetChanCtrl");

    ASSERTMSGLINE(0x37F, chan >= 0 && chan <= 5, "GXSetChanCtrl: Invalid Channel Id");

#if DEBUG
    if (chan == 4)
        idx = 0;
    else if (chan == 5)
        idx = 1;
    else
        idx = chan;
#else
    idx = chan & 0x3;
#endif

    reg = 0;
    SET_REG_FIELD(0x38B, reg, 1, 1, enable);
    SET_REG_FIELD(0x38C, reg, 1, 0, mat_src);
    SET_REG_FIELD(0x38D, reg, 1, 6, amb_src);
    
    SET_REG_FIELD(0x38F, reg, 2, 7, (attn_fn == 0) ? 0 : diff_fn);
    SET_REG_FIELD(0x390, reg, 1, 9, (attn_fn != 2));
    SET_REG_FIELD(0x391, reg, 1, 10, (attn_fn != 0));

    SET_REG_FIELD(0x39D, reg, 4, 2, light_mask & 0xF);
    SET_REG_FIELD(0x39E, reg, 4, 11, (light_mask >> 4) & 0xF);

    GX_WRITE_XF_REG(idx + 14, reg);
    
    if (chan == GX_COLOR0A0) {
        GX_WRITE_XF_REG(16, reg);
    } else if (chan == GX_COLOR1A1) {
        GX_WRITE_XF_REG(17, reg);
    }

    __GXData->bpSentNot = 1;
}
