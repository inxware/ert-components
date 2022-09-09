
public interface ITextObjectHandler
{
    ITextObjectHandler Init(EHS_TextObjectConfig objectConfig);

    ITextObjectHandler Update(EHS_TextConfig textConfig, string text);

    ITextObjectHandler Clear();
}
