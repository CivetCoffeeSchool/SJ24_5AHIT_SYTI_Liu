namespace Messomat4kVisualisierung.Dto;

public class AGraphData
{ 
    public AGraphData(float data, int serialNumber)
    {
        Data = data;
        SerialNumber = serialNumber;
    }
    public float Data { get; init; }
    public int SerialNumber { get; init; }
}