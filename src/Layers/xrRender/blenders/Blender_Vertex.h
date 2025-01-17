#pragma once

class CBlender_Vertex : public IBlender
{
    xrP_TOKEN oTessellation;

private:
    void CompileFFP(CBlender_Compile& C) const;
    void CompileProgrammable(CBlender_Compile& C) const;
public:
    CBlender_Vertex();
    ~CBlender_Vertex() override = default;

    LPCSTR getComment() override;
    BOOL canBeDetailed() override;
    
    void Save(IWriter& fs) override;
    void Load(IReader& fs, u16 version) override;
    
    void Compile(CBlender_Compile& C) override;
};
