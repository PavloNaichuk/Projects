namespace Task5.Data
{
    public interface IStringSerializer<TEntity>
    {
        TEntity Deserialize(string strEntity);
        string Serialize(TEntity Entity);
    }
}
