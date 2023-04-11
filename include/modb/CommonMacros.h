#ifndef COMMONMACROS_H
#define COMMONMACROS_H

#define __SERIALIZE_DATA__ template <class Archive> \
        inline void serialize(Archive& ar, unsigned int) { \
            ar& data;\
        }\

#define __DATARESOURCE_TEMPLATE__ template std::string& Serializer<DATATYPE>::Serialize(DATATYPE data);\
template DATATYPE& Serializer<DATATYPE>::Deserialize(std::string& serializedData);\
template Serializer<DATATYPE>&  DatabaseResource<DATATYPE>::Serializer_();\
template Serializer<DATATYPE>::~Serializer();\
template std::string& Serializer<DATATYPE>::GetSerializedData();\
template DATATYPE& Serializer<DATATYPE>::GetData();\
template void DatabaseResource<DATATYPE>::m_SetDBPoint(Db* db);\
template void DatabaseResource<DATATYPE>::m_InstantiateSerializer();\
template DatabaseResource<DATATYPE>::DatabaseResource(); \
template DatabaseResource<DATATYPE>::DatabaseResource(const std::string& dbName, DBTYPE type);\
template void DatabaseResource<DATATYPE>::m_ExceptionForOpening(); \
template void DatabaseResource<DATATYPE>::WriteKeyValuePair(const std::string& key, const std::string& value, modb::RECORD_WRITE_OPTION status);\
template Dbt* DatabaseResource<DATATYPE>::m_ConvertDbt(const std::string& value) ;\
template DATATYPE& DatabaseResource<DATATYPE>::FindById(const std::string& key);\
template DatabaseResource<DATATYPE>::DatabaseResource(Db* database) ;\
template DatabaseResource<DATATYPE>& DatabaseResource<DATATYPE>::operator=(std::nullptr_t);\
template void DatabaseResource<DATATYPE>::SetErrorStream(__DB_STD(ostream) *error_stream);\
template void DatabaseResource<DATATYPE>::m_SafeModLog(const std::string& logMessage);\
template void DatabaseResource<DATATYPE>::Open(DBTYPE type) ;\

#endif
