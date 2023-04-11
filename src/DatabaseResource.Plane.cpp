// this file purpose increasing compilation performance. other option inlinend header method
#if __INCLUDE_LEVEL__
    #error "Don't include this file"
#endif

#include <modb/DatabaseResource.inl>
#include <modb/DatabaseResource.h>
#include <modb/Plane.h>
#include <modb/Rectangle.h>
using namespace modb;

// only change here for implement new datatype for DataResource template 
#define DATATYPE Plane


template std::string& Serializer<DATATYPE>::Serialize(DATATYPE data);
template Plane& Serializer<DATATYPE>::Deserialize(std::string& serializedData);
template Serializer<DATATYPE>&  DatabaseResource<DATATYPE>::Serializer_();

template Serializer<DATATYPE>::~Serializer();
template std::string& Serializer<DATATYPE>::GetSerializedData();
template DATATYPE& Serializer<DATATYPE>::GetData();

template void DatabaseResource<DATATYPE>::m_SetDBPoint(Db* db);
template void DatabaseResource<DATATYPE>::m_InstantiateSerializer();
template DatabaseResource<DATATYPE>::DatabaseResource(); 
template DatabaseResource<DATATYPE>::DatabaseResource(const std::string& dbName, DBTYPE type);
template void DatabaseResource<DATATYPE>::m_ExceptionForOpening(); 
template void DatabaseResource<DATATYPE>::WriteKeyValuePair(const std::string& key, const std::string& value);
template Dbt* DatabaseResource<DATATYPE>::m_ConvertDbt(const std::string& value) ;
template DATATYPE& DatabaseResource<DATATYPE>::FindById(const std::string& key);
template DatabaseResource<DATATYPE>::DatabaseResource(Db* database) ;
template DatabaseResource<DATATYPE>& DatabaseResource<DATATYPE>::operator=(std::nullptr_t);
template void DatabaseResource<DATATYPE>::SetErrorStream(__DB_STD(ostream) *error_stream);
template void DatabaseResource<DATATYPE>::m_SafeModLog(const std::string& logMessage);
template void DatabaseResource<DATATYPE>::Open(DBTYPE type) ;
// template DatabaseResource<Plane>::~DatabaseResource();