bool storeData(int v)
{
  
  data[indexData++]=(short)v;

  return indexData>=MAX_DATA;
}

int computeNoiseValueAverage()
{
  long sum=0;
  
  for(int i=0;i<MAX_DATA;i++)
  {
    sum+=data[i];
  }

  int media=sum/MAX_DATA;

  Serial.println("***********************************************");
  Serial.println((String)"MEDIA Avg: "+media+". Total Sum:"+sum);
  Serial.println("***********************************************");

  return media;
}
