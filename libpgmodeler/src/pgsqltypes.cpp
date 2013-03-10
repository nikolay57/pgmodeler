#include "pgsqltypes.h"

/********************
 * CLASS: BaseType  *
 ********************/
QString BaseType::type_list[types_count]=
{
	/* Tipo vazio, quando se instância a classe TipoBase ao usar o operador ~
			este será o único tipo retornado */
	//offsets 0
	"",

	//Types used by the class ActionType
	//offsets 1 to 5
	"NO ACTION",
	"RESTRICT",
	"CASCADE",
	"SET NULL",
	"SET DEFAULT",

	//Types used by the class ConstraintType
	//offsets 6 to 9
	"PRIMARY KEY",
	"FOREIGN KEY",
	"CHECK",
	"UNIQUE",

	//Types used by the class EventType
	//offsets 10 to 14
	"ON SELECT",
	"ON INSERT",
	"ON DELETE",
	"ON UPDATE",
	"ON TRUNCATE",

	//Types used by the class ExecutionType
	//offsets 15 to 16
	"ALSO",
	"INSTEAD",

	//Types used by the class FunctionType
	//offsets 17 to 19
	"VOLATILE",
	"STABLE",
	"IMMUTABLE",

	//Types used by the class IndexingType
	//offsets 20 to 24
	"btree",
	"rtree",
	"gist",
	"hash",
	"gin",

	//Types used by the class PgSQLType
	//offsets 25 to 63
	"smallint", "integer", "bigint", "decimal", "numeric",
	"real", "double precision", "float", "serial", "bigserial", "money",
	"character varying", "varchar", "character",
	"char", "text", "bytea",
	"timestamp", "date", "time","timetz","timestamptz",
	"interval", "boolean",
	"point", "line", "lseg", "box", "path",
	"polygon", "circle", "cidr", "inet",
	"macaddr", "bit", "bit varying", "varbit", "uuid", "xml",

	//Spatial type specifics for the PostGiS extension
	//offsets 64 to 68
	"box2d","box3d","geometry",
	"geometry_dump","geography",

	//Object Identification type (OID)
	//offsets 69 to 80
	"oid", "regproc", "regprocedure",
	"regoper", "regoperator", "regclass",
	"regtype", "regconfig", "regdictionary",
	"xid", "cid", "tid",

	//Pseudo-types
	//offsets 81 to 92
	"any","anyarray","anyelement","anyenum",
	"anynonarray","cstring","internal","language_handler",
	"record","trigger","void","opaque",

	//Interval types
	//offsets 93 to 105
	"YEAR", "MONTH", "DAY", "HOUR",
	"MINUTE", "SECOND","YEAR TO MONTH",
	"DAY TO HOUR","DAY TO MINUTE","DAY TO SECOND",
	"HOUR TO MINUTE","HOUR TO SECOND","MINUTE TO SECOND",

	//Types used by the class BehaviorType
	//offsets 106 to 108
	"CALLED ON NULL INPUT",
	"RETURNS NULL ON NULL INPUT",
	"STRICT",

	//Types used by the class SecurityType
	//offsets 109 to 110
	"SECURITY INVOKER",
	"SECURITY DEFINER",

	//Types used by the class LanguageType
	//offsets 111 to 116
	"sql",
	"c",
	"plpgsql",
	"pltcl",
	"plperl",
	"plpython",

	//Types used by the class EncodingType
	//offsets 117 to 157
	"UTF8", "BIG5", "EUC_CN",  "EUC_JP", "EUC_JIS_2004", "EUC_KR",
	"EUC_TW", "GB18030", "GBK", "ISO_8859_5", "ISO_8859_6",
	"ISO_8859_7", "ISO_8859_8", "JOHAB", "KOI", "LATIN1",
	"LATIN2", "LATIN3", "LATIN4", "LATIN5", "LATIN6",
	"LATIN7", "LATIN8", "LATIN9", "LATIN10", "MULE_INTERNAL",
	"SJIS", "SHIFT_JIS_2004", "SQL_ASCII", "UHC",
	"WIN866", "WIN874", "WIN1250", "WIN1251", "WIN1252",
	"WIN1253", "WIN1254", "WIN1255", "WIN1256", "WIN1257",
	"WIN1258",

	//Types used by the class StorageType
	//offsets 158 to 161
	"plain",
	"external",
	"extended",
	"main",

	//Types used by the class MatchType
	//offsets 162 to 164
	"MATCH FULL",
	"MATCH PARTIAL",
	"MATCH SIMPLE",

	//Types used by the class DeferralType
	//offsets 165 to 166
	"INITIALLY IMMEDIATE",
	"INITIALLY DEFERRED",

	//Types used by the class CategoryType
	//offsets 167 to 180 - See table 44-43 on PostgreSQL 8.4 documentation
	"U", //User-defined types
	"A", //Array types
	"B", //Boolean types
	"C", //Composite types
	"D", //Date/time types
	"E", //Enum types
	"G", //Geometric types
	"I", //Network address types
	"N", //Numeric types
	"P", //Pseudo-types
	"S", //String types
	"T", //Timespan types
	"V", //Bit-string types
	"X", //Unknown type

	//Types used by the class FiringType
	//offsets 181 to 183
	"BEFORE",
	"AFTER",
	"INSTEAD OF",

	/* Auxiliary types used by PostGiS types.
			These types accepts variations Z, M e ZM.
			 > Example: POINT, POINTZ, POINTM, POINTZM
			Reference: http://postgis.refractions.net/documentation/manual-2.0/using_postgis_dbmanagement.html */
	//offsets 184 to 190
	"POINT",
	"LINESTRING",
	"POLYGON",
	"MULTIPOINT",
	"MULTILINESTRING",
	"MULTIPOLYGON",
	"GEOMETRYCOLLECTION"
};

BaseType::BaseType(void)
{
	type_idx=0;
}

QString BaseType::getTypeString(unsigned type_id)
{
	if(type_id > types_count)
		throw Exception(ERR_REF_TYPE_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(type_list[type_id]);
}

void BaseType::setType(unsigned type_id,unsigned offset,unsigned count)
{
	//Raises an error if the type count is invalid
	if(count==0 || count > this->types_count)
		throw Exception(ERR_OBT_TYPES_INV_QUANTITY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the type id is invalid
	else if(!isTypeValid(type_id,offset,count))
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		type_idx=type_id;
}

bool BaseType::isTypeValid(unsigned type_id,unsigned offset,unsigned count)
{
	//Returns if the type id is valid according to the specified interval (offset-count)
	return((type_id>=offset && type_id<=(offset+count-1)) || type_id==0);
}

void BaseType::getTypes(QStringList &types,unsigned offset,unsigned count)
{
	//Raises an error if the type count is invalid
	if(count==0 || count > BaseType::types_count)
		throw Exception(ERR_OBT_TYPES_INV_QUANTITY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		unsigned idx,total;

		types.clear();
		total=offset+count;

		for(idx=offset; idx<total; idx++)
			types.push_back(BaseType::type_list[idx]);
	}
}

unsigned BaseType::getType(const QString &type_name,unsigned offset,unsigned count)
{
	unsigned idx,total;
	bool found=false;

	if(type_name=="")
		return(BaseType::null);
	else
	{
		total=offset + count;

		for(idx=offset; idx<total && !found; idx++)
			found=(type_name==BaseType::type_list[idx]);

		if(found)
		{ idx--; return(idx); }
		else
			return(BaseType::null);
	}
}

QString BaseType::operator ~ (void)
{
	return(type_list[type_idx]);
}

unsigned BaseType::operator ! (void)
{
	return(type_idx);
}

bool BaseType::operator == (BaseType &type)
{
	return(type.type_idx == type_idx);
}

bool BaseType::operator == (unsigned type_id)
{
	return(type_idx==type_id);
}

bool BaseType::operator != (BaseType &type)
{
	return(type.type_idx != type_idx);
}

bool BaseType::operator != (unsigned type_id)
{
	return(type_idx!=type_id);
}

/*********************
 * CLASS: ActionType *
 *********************/
ActionType::ActionType(unsigned type_id)
{
	(*this)=type_id;
}

ActionType::ActionType(const QString &type_name)
{
	(*this)=type_name;
}

ActionType::ActionType(void)
{
	type_idx=offset;
}

void ActionType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned ActionType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned ActionType::operator = (const QString &type_name)
{
	unsigned type_idx;

	type_idx=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_idx,offset,types_count);
	return(type_idx);
}

/*************************
 * CLASS: ConstraintType *
 *************************/
ConstraintType::ConstraintType(unsigned type_id)
{
	(*this)=type_id;
}

ConstraintType::ConstraintType(const QString &type_name)
{
	(*this)=type_name;
}

ConstraintType::ConstraintType(void)
{
	type_idx=offset;
}

void ConstraintType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned ConstraintType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned ConstraintType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/********************
 * CLASS: EventType *
 ********************/
EventType::EventType(void)
{
	type_idx=offset;
}

EventType::EventType(const QString &type_name)
{
	(*this)=type_name;
}

EventType::EventType(unsigned type_id)
{
	(*this)=type_id;
}

void EventType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned EventType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned EventType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

bool EventType::operator < (EventType type) const
{
	return(type_idx < type.type_idx);
}

bool EventType::operator < (unsigned type_id) const
{
	return(type_idx < type_id);
}

/************************
 * CLASS: ExecutionType *
 ************************/
ExecutionType::ExecutionType(void)
{
	type_idx=offset;
}

ExecutionType::ExecutionType(unsigned type_id)
{
	(*this)=type_id;
}

ExecutionType::ExecutionType(const QString &type_name)
{
	(*this)=type_name;
}

void ExecutionType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned ExecutionType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned ExecutionType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: FunctionType *
 ***********************/
FunctionType::FunctionType(unsigned type_id)
{
	(*this)=type_id;
}

FunctionType::FunctionType(void)
{
	type_idx=offset;
}

FunctionType::FunctionType(const QString &type_name)
{
	(*this)=type_name;
}

void FunctionType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned FunctionType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned FunctionType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: IndexingType *
 ***********************/
IndexingType::IndexingType(unsigned type_id)
{
	(*this)=type_id;
}

IndexingType::IndexingType(void)
{
	type_idx=offset;
}

IndexingType::IndexingType(const QString &type_name)
{
	(*this)=type_name;
}

void IndexingType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned IndexingType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned IndexingType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: IntervalType *
 ***********************/
IntervalType::IntervalType(unsigned type_id)
{
	(*this)=type_id;
}

IntervalType::IntervalType(void)
{
	type_idx=BaseType::null;
}

IntervalType::IntervalType(const QString &type_name)
{
	(*this)=type_name;
}

void IntervalType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned IntervalType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned IntervalType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/**********************
 * CLASS: SpatialType *
 **********************/
SpatialType::SpatialType(const QString &type_name, int srid, unsigned variation_id)
{
	BaseType::setType(BaseType::getType(type_name, offset, types_count),
										offset, types_count);
	setVariation(variation_id);
	setSRID(srid);
}

SpatialType::SpatialType(unsigned type_id, int srid, unsigned var_id)
{
	BaseType::setType(type_id,offset,types_count);
	setVariation(var_id);
	setSRID(srid);
}

SpatialType::SpatialType(void)
{
	type_idx=point;
	variacao=no_var;
}

void SpatialType::setVariation(unsigned var)
{
	if(var > var_zm)
		variacao=var_zm;
	else
		variacao=var;
}

unsigned SpatialType::getVariation(void)
{
	return(variacao);
}

void SpatialType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

void SpatialType::setSRID(int srid)
{
	if(srid < -1) srid=-1;
	this->srid=srid;
}

int SpatialType::getSRID(void)
{
	return(srid);
}

QString SpatialType::operator * (void)
{
	QString var_str;

	switch(variacao)
	{
		case var_z: var_str+="Z"; break;
		case var_m: var_str+="M"; break;
		case var_zm: var_str+="ZM"; break;
		default: var_str=""; break;
	}

	return(QString("(%1%2, %3)").arg(type_list[type_idx]).arg(var_str)).arg(srid);
}

/********************
 * CLASS: PgSQLType *
 ********************/
vector<UserTypeConfig> PgSQLType::user_types;

PgSQLType::PgSQLType(void)
{
	type_idx=offset;
	length=1;
	precision=-1;
	dimension=0;
	with_timezone=false;
}

PgSQLType::PgSQLType(const QString &type_name)
{
	(*this)=type_name;
	length=1;
	precision=-1;
	dimension=0;
	with_timezone=false;
}

PgSQLType::PgSQLType(void *ptype)
{
	(*this) << ptype;
	length=1;
	precision=-1;
	dimension=0;
	with_timezone=false;
}

PgSQLType::PgSQLType(void *ptype, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type)
{
	(*this) << ptype;
	setLength(length);
	setDimension(dimension);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSQLType::PgSQLType(const QString &type_name, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type)
{
	(*this)=type_name;
	setLength(length);
	setDimension(dimension);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

PgSQLType::PgSQLType(unsigned type_id, unsigned length, unsigned dimension, int precision, bool with_timezone, IntervalType interv_type, SpatialType spatial_type)
{
	(*this)=type_id;
	setLength(length);
	setDimension(dimension);
	setPrecision(precision);
	setWithTimezone(with_timezone);
	setIntervalType(interv_type);
	setSpatialType(spatial_type);
}

void PgSQLType::getTypes(QStringList &type_list, bool oids, bool pseudos)
{
	unsigned idx,total;

	type_list.clear();
	total=offset+types_count;

	for(idx=offset; idx<total; idx++)
	{
		if(idx<oid_start ||
			 (oids && idx>=oid_start && idx<=oid_end) ||
			 (pseudos && idx>=pseudo_start && idx<=pseudo_end))
			type_list.push_back(BaseType::type_list[idx]);
	}
}

unsigned PgSQLType::operator = (unsigned type_id)
{
	if(type_id>=offset)
		setUserType(type_id);
	else if(type_id > 0)
		BaseType::setType(type_id,offset,types_count);
	else if(type_id==0)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(type_idx);
}

unsigned PgSQLType::operator = (const QString &type_name)
{
	unsigned type_idx, usr_type_idx;

	type_idx=BaseType::getType(type_name, offset, types_count);
	usr_type_idx=getUserTypeIndex(type_name, NULL);

	if(type_idx==0 && usr_type_idx==0)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else if(type_idx!=0)
	{
		BaseType::setType(type_idx,offset,types_count);
		return(type_idx);
	}
	else
	{
		setUserType(usr_type_idx);
		return(usr_type_idx);
	}
}

void *PgSQLType::getUserTypeReference(void)
{
	if(this->isUserType())
		return(user_types[this->type_idx - (pseudo_end + 1)].ptype);
	else
		return(NULL);
}

unsigned PgSQLType::getUserTypeConfig(void)
{
	if(this->isUserType())
		return(user_types[this->type_idx - (pseudo_end + 1)].type_conf);
	else
		return(0);
}

bool PgSQLType::operator == (unsigned type_id)
{
	return(this->type_idx==type_id);
}

bool PgSQLType::operator == (const QString &type_name)
{
	unsigned idx,total;
	bool found=false;

	total=offset + types_count;

	for(idx=offset; idx<total && !found; idx++)
		found=(type_name==BaseType::type_list[idx]);

	if(found) idx--;

	return(type_idx==idx);
}

bool PgSQLType::operator != (const QString &type_name)
{
	return(!((*this)==type_name));
}

bool PgSQLType::operator != (PgSQLType type)
{
	return(this->type_idx!=type.type_idx);
}

bool PgSQLType::operator != (unsigned type_id)
{
	return(this->type_idx!=type_id);
}

bool PgSQLType::operator == (PgSQLType type)
{
	return(this->type_idx==type.type_idx);
}

bool PgSQLType::operator == (void *ptype)
{
	int idx;
	idx=getUserTypeIndex("",ptype);
	return(static_cast<int>(type_idx) == idx);
}

IntervalType PgSQLType::getIntervalType(void)
{
	return(interval_type);
}

SpatialType PgSQLType::getSpatialType(void)
{
	return(spatial_type);
}

bool PgSQLType::isWithTimezone(void)
{
	return(with_timezone);
}

bool PgSQLType::isOIDType(void)
{
	return(type_idx>=oid_start && type_idx<=oid_end);
}

bool PgSQLType::isPseudoType(void)
{
	return(type_idx>=pseudo_start && type_idx<=pseudo_end);
}

unsigned PgSQLType::operator << (void *ptype)
{
	setUserType(ptype);
	return(type_idx);
}

void PgSQLType::setIntervalType(IntervalType interv_type)
{
	interval_type=interv_type;
}

void PgSQLType::setSpatialType(SpatialType spat_type)
{
	spatial_type=spat_type;
}

void PgSQLType::setWithTimezone(bool with_tz)
{
	this->with_timezone=with_tz;
}

void PgSQLType::setUserType(unsigned type_id)
{
	unsigned lim1, lim2;

	lim1=pseudo_end + 1;
	lim2=lim1 + PgSQLType::user_types.size();

	if(PgSQLType::user_types.size() > 0 &&
		 (type_id >= lim1 && type_id < lim2))
		type_idx=type_id;
	else
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void PgSQLType::setUserType(void *ptype)
{
	int idx;

	idx=getUserTypeIndex("",ptype);
	if(idx <= 0)
		throw Exception(ERR_ASG_INV_TYPE_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
		type_idx=idx;
}

void PgSQLType::addUserType(const QString &type_name, void *ptype, void *pmodel, unsigned type_conf)
{
	if(type_name!="" && ptype && pmodel &&
		 (type_conf==UserTypeConfig::DOMAIN_TYPE ||
			type_conf==UserTypeConfig::SEQUENCE_TYPE ||
			type_conf==UserTypeConfig::TABLE_TYPE ||
			type_conf==UserTypeConfig::BASE_TYPE) &&
		 getUserTypeIndex(type_name,ptype,pmodel)==0)
	{
		UserTypeConfig cfg;

		cfg.name=type_name;
		cfg.ptype=ptype;
		cfg.pmodel=pmodel;
		cfg.type_conf=type_conf;
		PgSQLType::user_types.push_back(cfg);
	}
}

void PgSQLType::removeUserType(const QString &type_name, void *ptype)
{
	if(PgSQLType::user_types.size() > 0 &&
		 type_name!="" && ptype)
	{
		UserTypeConfig cfg;
		vector<UserTypeConfig>::iterator itr, itr_end;

		itr=PgSQLType::user_types.begin();
		itr_end=PgSQLType::user_types.end();

		while(itr!=itr_end)
		{
			if(itr->name==type_name && itr->ptype==ptype) break;
			else itr++;
		}

		if(itr!=itr_end)
			PgSQLType::user_types.erase(itr);
	}
}

void PgSQLType::renameUserType(const QString &type_name, void *ptype,const QString &new_name)
{
	if(PgSQLType::user_types.size() > 0 &&
		 type_name!="" && ptype && type_name!=new_name)
	{
		vector<UserTypeConfig>::iterator itr, itr_end;

		itr=PgSQLType::user_types.begin();
		itr_end=PgSQLType::user_types.end();

		while(itr!=itr_end)
		{
			if(itr->name==type_name && itr->ptype==ptype)
			{
				itr->name=new_name;
				break;
			}
			itr++;
		}
	}
}

unsigned PgSQLType::getBaseTypeIndex(const QString &type_name)
{
	return(getType(type_name,offset,types_count));
}

unsigned PgSQLType::getUserTypeIndex(const QString &type_name, void *ptype, void *pmodel)
{
	if(PgSQLType::user_types.size() > 0 && (type_name!="" || ptype))
	{
		vector<UserTypeConfig>::iterator itr, itr_end;
		int idx=0;

		itr=PgSQLType::user_types.begin();
		itr_end=PgSQLType::user_types.end();

		while(itr!=itr_end)
		{
			if(((type_name!="" && itr->name==type_name) || (ptype && itr->ptype==ptype)) &&
				 ((pmodel && itr->pmodel==pmodel) || !pmodel))
				break;

			idx++;
			itr++;
		}

		if(itr!=itr_end)
			return(pseudo_end + 1 + idx);
		else
			return(BaseType::null);
	}
	else return(BaseType::null);
}

QString PgSQLType::getUserTypeName(unsigned type_id)
{
	unsigned lim1, lim2;

	lim1=pseudo_end + 1;
	lim2=lim1 + PgSQLType::user_types.size();


	if(PgSQLType::user_types.size() > 0 &&
		 (type_id >= lim1 && type_id < lim2))
		return(PgSQLType::user_types[type_id - lim1].name);
	else
		return("");
}

void PgSQLType::getUserTypes(QStringList &type_list, void *pmodel, unsigned inc_usr_types)
{
	unsigned idx,total;

	type_list.clear();
	total=PgSQLType::user_types.size();

	for(idx=0; idx < total; idx++)
	{
		//Only the user defined types of the specified model are retrieved
		if(user_types[idx].pmodel==pmodel &&
			 ((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
			type_list.push_back(user_types[idx].name);
	}
}

void PgSQLType::getUserTypes(vector<void *> &ptypes, void *pmodel, unsigned inc_usr_types)
{
	unsigned idx, total;

	ptypes.clear();
	total=PgSQLType::user_types.size();

	for(idx=0; idx < total; idx++)
	{
		//Only the user defined types of the specified model are retrieved
		if(user_types[idx].pmodel==pmodel &&
			 ((inc_usr_types & user_types[idx].type_conf) == user_types[idx].type_conf))
			ptypes.push_back(user_types[idx].ptype);
	}
}

QString PgSQLType::operator ~ (void)
{
	if(type_idx >= pseudo_end + 1)
		return(user_types[type_idx - (pseudo_end + 1)].name);
	else
		return(BaseType::type_list[type_idx]);
}

bool PgSQLType::isArrayType(void)
{
	return(dimension > 0);
}

bool PgSQLType::isUserType(void)
{
	return(type_idx > pseudo_end);
}

bool PgSQLType::isGiSType(void)
{
	return(type_list[this->type_idx]=="geography" ||
			type_list[this->type_idx]=="geometry" ||
			type_list[this->type_idx]=="geometry_dump");
}

bool PgSQLType::hasVariableLength(void )
{
	return(type_list[this->type_idx]=="numeric" || type_list[this->type_idx]=="decimal" ||
			type_list[this->type_idx]=="character varying" || type_list[this->type_idx]=="varchar" ||
			type_list[this->type_idx]=="character" || type_list[this->type_idx]=="char" ||
			type_list[this->type_idx]=="bit" || type_list[this->type_idx]=="bit varying" ||
			type_list[this->type_idx]=="varbit");
}

bool PgSQLType::acceptsPrecision(void )
{
	return(type_list[this->type_idx]=="numeric" || type_list[this->type_idx]=="decimal" ||
			type_list[this->type_idx]=="time" || type_list[this->type_idx]=="timestamp" ||
			type_list[this->type_idx]=="interval");
}

void PgSQLType::setDimension(unsigned dim)
{
	if(dim > 0 && this->isUserType())
	{
		int idx=getUserTypeIndex(~(*this), NULL);
		if(user_types[idx].type_conf==UserTypeConfig::DOMAIN_TYPE ||
			 user_types[idx].type_conf==UserTypeConfig::SEQUENCE_TYPE)
			throw Exception(ERR_ASG_INV_DOMAIN_ARRAY,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}

	dimension=dim;
}

void PgSQLType::setLength(unsigned len)
{
	//Raises an error if the length is 0
	if(len==0)
		throw Exception(ERR_ASG_ZERO_LENGTH,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->length=len;
}

void PgSQLType::setPrecision(int prec)
{
	//Raises an error if the user tries to specify a precision > lenght
	if(((BaseType::type_list[type_idx]=="numeric" ||
			 BaseType::type_list[type_idx]=="decimal") && prec > static_cast<int>(length)))
		throw Exception(ERR_ASG_INV_PRECISION,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	//Raises an error if the precision is greater thant 6
	else if(((BaseType::type_list[type_idx]=="time" ||
						BaseType::type_list[type_idx]=="timestamp" ||
						BaseType::type_list[type_idx]=="interval") && prec > 6))
		throw Exception(ERR_ASG_INV_PREC_TIMESTAMP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	this->precision=prec;
}

unsigned PgSQLType::getDimension(void)
{
	return(dimension);
}

unsigned PgSQLType::getLength(void)
{
	return(length);
}

int PgSQLType::getPrecision(void)
{
	return(precision);
}

QString PgSQLType::getCodeDefinition(unsigned def_type,QString ref_type)
{
	if(def_type==SchemaParser::SQL_DEFINITION)
		return(*(*this));
	else
	{
		map<QString, QString> attribs;

		attribs[ParsersAttributes::LENGTH]="";
		attribs[ParsersAttributes::DIMENSION]="";
		attribs[ParsersAttributes::PRECISION]="";
		attribs[ParsersAttributes::WITH_TIMEZONE]="";
		attribs[ParsersAttributes::INTERVAL_TYPE]="";
		attribs[ParsersAttributes::SPATIAL_TYPE]="";
		attribs[ParsersAttributes::VARIATION]="";
		attribs[ParsersAttributes::SRID]="";
		attribs[ParsersAttributes::REF_TYPE]=ref_type;

		attribs[ParsersAttributes::NAME]=(~(*this));

		if(length > 1)
			attribs[ParsersAttributes::LENGTH]=QString("%1").arg(this->length);

		if(dimension > 0)
			attribs[ParsersAttributes::DIMENSION]=QString("%1").arg(this->dimension);

		if(precision >= 0)
			attribs[ParsersAttributes::PRECISION]=QString("%1").arg(this->precision);

		if(interval_type != BaseType::null)
			attribs[ParsersAttributes::INTERVAL_TYPE]=(~interval_type);

		if(isGiSType())
		{
			attribs[ParsersAttributes::SPATIAL_TYPE]=(~spatial_type);
			attribs[ParsersAttributes::VARIATION]=QString("%1").arg(spatial_type.getVariation());
			attribs[ParsersAttributes::SRID]=QString("%1").arg(spatial_type.getSRID());
		}

		if(with_timezone)
			attribs[ParsersAttributes::WITH_TIMEZONE]="1";

		return(SchemaParser::getCodeDefinition("basetype",attribs, def_type));
	}
}

QString PgSQLType::operator * (void)
{
	QString fmt_type, type, aux;
	unsigned idx;

	type=~(*this);

	//Generation the definition for the spatial types (PostGiS)
	if(type=="geometry" || type=="geography")
		fmt_type=type + (*spatial_type);
	else if(length > 1 && hasVariableLength())
	{
		//Configuring the precision
		if((type=="numeric" || type=="decimal") && precision>=0 &&
			 precision<=static_cast<int>(length))
			aux=QString("%1(%2,%3)").arg(BaseType::type_list[type_idx]).arg(length).arg(precision);
		//Configuring the length for the type
		else
			aux=QString("%1(%2)").arg(BaseType::type_list[type_idx]).arg(length);

		fmt_type=aux;
	}
	else if(type!="numeric" && type!="decimal" && acceptsPrecision())
	{
		if(type!="interval")
		{
			aux=BaseType::type_list[type_idx];

			if(precision >= 0)
				aux+=QString("(%1)").arg(precision);

			if(with_timezone)
				aux+=" with time zone";
		}
		else
		{
			aux=BaseType::type_list[type_idx];

			if(interval_type!=BaseType::null)
				aux+=QString("[%1]").arg(~interval_type);

			if(precision >= 0)
				aux+=QString("(%1)").arg(precision);
		}

		fmt_type=aux;
	}
	else
		fmt_type=type;


	if(type!="void" && dimension > 0)
	{
		for(idx=0; idx < dimension; idx++)
			fmt_type+="[]";
	}

	return(fmt_type);
}

/***********************
 * CLASS: BehaviorType *
 ***********************/
BehaviorType::BehaviorType(unsigned type_id)
{
	(*this)=type_id;
}

BehaviorType::BehaviorType(void)
{
	type_idx=offset;
}

BehaviorType::BehaviorType(const QString &type_name)
{
	(*this)=type_name;
}

void BehaviorType::getTypes(QStringList &type_list)
{
	BaseType::getTypes(type_list,offset,types_count);
}

unsigned BehaviorType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned BehaviorType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: SecurityType *
 ***********************/
SecurityType::SecurityType(void)
{
	type_idx=offset;
}

SecurityType::SecurityType(const QString &type_name)
{
	(*this)=type_name;
}

SecurityType::SecurityType(unsigned type_id)
{
	(*this)=type_id;
}

void SecurityType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned SecurityType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned SecurityType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: LanguageType *
 ***********************/
LanguageType::LanguageType(void)
{
	type_idx=offset;
}

LanguageType::LanguageType(unsigned type_id)
{
	(*this)=type_id;
}

LanguageType::LanguageType(const QString &type_name)
{
	(*this)=type_name;
}

void LanguageType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned LanguageType::operator = (unsigned tipo_id)
{
	BaseType::setType(tipo_id,offset,types_count);
	return(type_idx);
}

unsigned LanguageType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: EncodingType *
 ***********************/
EncodingType::EncodingType(void)
{
	type_idx=offset;
}

EncodingType::EncodingType(const QString &type)
{
	(*this)=type;
}

void EncodingType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned EncodingType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned EncodingType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

bool EncodingType::operator == (const char *type_name)
{
	return((*this)==QString(type_name));
}

bool EncodingType::operator == (const QString &type_name)
{
	unsigned idx,total;
	bool found=false;

	total=offset + types_count;

	for(idx=offset; idx<total && !found; idx++)
		found=(type_name==BaseType::type_list[idx]);

	if(found) idx--;

	return(type_idx==idx);
}

bool EncodingType::operator != (const QString &type_name)
{
	return(!((*this)==type_name));
}

bool EncodingType::operator != (EncodingType type)
{
	return(this->type_idx!=type.type_idx);
}

/**********************
 * CLASS: StorageType *
 **********************/
StorageType::StorageType(void)
{
	type_idx=offset;
}

StorageType::StorageType(const QString &type_name)
{
	(*this)=type_name;
}

void StorageType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned StorageType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned StorageType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

bool StorageType::operator == (const char *type_name)
{
	return((*this)==QString(type_name));
}

bool StorageType::operator == (const QString &type_name)
{
	unsigned idx,total;
	bool found=false;

	total=offset + types_count;

	for(idx=offset; idx<total && !found; idx++)
		found=(type_name==BaseType::type_list[idx]);

	if(found) idx--;

	return(type_idx==idx);
}

bool StorageType::operator != (const QString &type_name)
{
	return(!((*this)==type_name));
}

bool StorageType::operator != (StorageType type)
{
	return(this->type_idx!=type.type_idx);
}

/********************
 * CLASS: MatchType *
 ********************/
MatchType::MatchType(void)
{
	type_idx=offset;
}

MatchType::MatchType(const QString &type_name)
{
	(*this)=type_name;
}

MatchType::MatchType(unsigned type_id)
{
	(*this)=type_id;
}

void MatchType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned MatchType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned MatchType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: DeferralType *
 ***********************/
DeferralType::DeferralType(void)
{
	type_idx=offset;
}

DeferralType::DeferralType(const QString &type_name)
{
	(*this)=type_name;
}

DeferralType::DeferralType(unsigned type_id)
{
	(*this)=type_id;
}

void DeferralType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned DeferralType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned DeferralType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/***********************
 * CLASS: CategoryType *
 ***********************/
CategoryType::CategoryType(void)
{
	type_idx=offset;
}

CategoryType::CategoryType(const QString &type_name)
{
	(*this)=type_name;
}

CategoryType::CategoryType(unsigned type_id)
{
	(*this)=type_id;
}

void CategoryType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned CategoryType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned CategoryType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

/*********************
 * CLASS: FiringType *
 *********************/
FiringType::FiringType(void)
{
	type_idx=offset;
}

FiringType::FiringType(unsigned type_id)
{
	(*this)=type_id;
}

FiringType::FiringType(const QString &type_name)
{
	(*this)=type_name;
}

void FiringType::getTypes(QStringList &tipos)
{
	BaseType::getTypes(tipos,offset,types_count);
}

unsigned FiringType::operator = (unsigned type_id)
{
	BaseType::setType(type_id,offset,types_count);
	return(type_idx);
}

unsigned FiringType::operator = (const QString &type_name)
{
	unsigned type_id;

	type_id=BaseType::getType(type_name, offset, types_count);
	BaseType::setType(type_id,offset,types_count);
	return(type_id);
}

