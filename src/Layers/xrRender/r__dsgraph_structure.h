#pragma once

#include "r__sector.h"

// feedback	for receiving visuals
class R_feedback
{
public:
    virtual void rfeedback_static(dxRender_Visual* V) = 0;
};

struct R_dsgraph_structure
{
    R_feedback* val_feedback{}; // feedback for geometry being rendered
    u32 val_feedback_breakp{}; // breakpoint
    xr_vector<Fbox3>* val_recorder; // coarse structure recorder

    u32 phase{};
    u32 marker{};
    bool pmask[2];
    bool pmask_wmark;

    // Dynamic scene graph
    // R_dsgraph::mapNormal_T										mapNormal	[2]		;	// 2==(priority/2)
    R_dsgraph::mapNormalPasses_T mapNormalPasses[2]; // 2==(priority/2)
    // R_dsgraph::mapMatrix_T										mapMatrix	[2]		;
    R_dsgraph::mapMatrixPasses_T mapMatrixPasses[2];
    R_dsgraph::mapSorted_T mapSorted;
    R_dsgraph::mapHUD_T mapHUD;
    R_dsgraph::mapLOD_T mapLOD;
    R_dsgraph::mapSorted_T mapDistort;
    R_dsgraph::mapHUD_T    mapHUDSorted;

#if RENDER != R_R1
    R_dsgraph::mapSorted_T mapWmark; // sorted
    R_dsgraph::mapSorted_T mapEmissive;
    R_dsgraph::mapSorted_T mapHUDEmissive;
#endif

    xr_vector<CSector*> Sectors;
    xr_vector<CPortal*> Portals;
    CPortalTraverser PortalTraverser;

    // Runtime structures
    xr_vector<R_dsgraph::mapNormal_T::value_type*> nrmPasses;
    xr_vector<R_dsgraph::mapMatrix_T::value_type*> matPasses;
    xr_vector<R_dsgraph::_LodItem> lstLODs;
    xr_vector<int> lstLODgroups;
    xr_vector<ISpatial*> lstRenderables;
    xr_vector<ISpatial*> lstSpatial;
    xr_vector<dxRender_Visual*> lstVisuals;

    u32 counter_S{};
    u32 counter_D{};

    void set_Feedback(R_feedback* V, u32 id)
    {
        val_feedback_breakp = id;
        val_feedback = V;
    }
    void set_Recorder(xr_vector<Fbox3>* dest)
    {
        val_recorder = dest;
        if (dest)
            dest->clear();
    }
    void get_Counters(u32& s, u32& d)
    {
        s = counter_S;
        d = counter_D;
    }
    void clear_Counters() { counter_S = counter_D = 0; }

    R_dsgraph_structure()
    {
        r_pmask(true, true);
    };

    void destroy()
    {
        nrmPasses.clear();
        matPasses.clear();

        lstLODs.clear();
        lstLODgroups.clear();
        lstRenderables.clear();
        lstSpatial.clear();
        lstVisuals.clear();

        for (int i = 0; i < SHADER_PASSES_MAX; ++i)
        {
            mapNormalPasses[0][i].clear();
            mapNormalPasses[1][i].clear();
            mapMatrixPasses[0][i].clear();
            mapMatrixPasses[1][i].clear();
        }
        mapSorted.clear();
        mapHUD.clear();
        mapLOD.clear();
        mapDistort.clear();
        mapHUDSorted.clear();

#if RENDER != R_R1
        mapWmark.clear();
        mapEmissive.clear();
        mapHUDEmissive.clear();
#endif
    }

    void r_pmask(bool _1, bool _2, bool _wm = false)
    {
        pmask[0] = _1;
        pmask[1] = _2;
        pmask_wmark = _wm;
    }

    void load(const xr_vector<CSector::level_sector_data_t> &sectors, const xr_vector<CPortal::level_portal_data_t> &portals);
    void unload();

    ICF IRender_Portal* get_portal(size_t id) const
    {
        VERIFY(id < Portals.size());
        return Portals[id];
    }
    ICF IRender_Sector* get_sector(size_t id) const
    {
        VERIFY(id < Sectors.size());
        return Sectors[id];
    }

    void add_static(dxRender_Visual* pVisual, const CFrustum& view, u32 planes);
    void add_leafs_dynamic(IRenderable* root, dxRender_Visual* pVisual, Fmatrix& xform); // if detected node's full visibility
    void add_leafs_static(dxRender_Visual* pVisual); // if detected node's full visibility

    void insert_dynamic(IRenderable* root, dxRender_Visual* pVisual, Fmatrix& xform, Fvector& Center);
    void insert_static(dxRender_Visual* pVisual);

    // render primitives
    void render_graph(u32 _priority);
    void render_hud();
    void render_hud_ui();
    void render_lods(bool _setup_zb, bool _clear);
    void render_sorted();
    void render_emissive();
    void render_wmarks();
    void render_distort();
    void render_subspace(IRender_Sector::sector_id_t sector_id, CFrustum* _frustum, Fmatrix& mCombined, Fvector& _cop,
        BOOL _dynamic, BOOL _precise_portals = FALSE);
    void render_subspace(IRender_Sector::sector_id_t sector_id, Fmatrix& mCombined, Fvector& _cop, BOOL _dynamic,
        BOOL _precise_portals = FALSE);
    void render_R1_box(u32 sector_id, Fbox& _bb, int _element);
};
