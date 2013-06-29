/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include "table.h"

Table::Table(void) : BaseTable()
{
	obj_type=OBJ_TABLE;
	with_oid=false;
	attributes[ParsersAttributes::COLUMNS]="";
	attributes[ParsersAttributes::CONSTRAINTS]="";
	attributes[ParsersAttributes::INDEXES]="";
	attributes[ParsersAttributes::TRIGGERS]="";
	attributes[ParsersAttributes::RULES]="";
	attributes[ParsersAttributes::OIDS]="";
	attributes[ParsersAttributes::COLS_COMMENT]="";
	attributes[ParsersAttributes::COPY_TABLE]="";
	copy_table=nullptr;
	this->setName(trUtf8("new_table").toUtf8());
}

Table::~Table(void)
{
	ObjectType types[]={ OBJ_TRIGGER, OBJ_INDEX, OBJ_RULE,
											 OBJ_CONSTRAINT, OBJ_COLUMN };
	vector<TableObject *> *list=nullptr;

	for(unsigned i=0; i < 5; i++)
	{
		list=getObjectList(types[i]);
		while(!list->empty())
		{
			delete(list->back());
			list->pop_back();
		}
	}

	ancestor_tables.clear();
}

void Table::setName(const QString &name)
{
	QString prev_name=this->getName(true);
	BaseObject::setName(name);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Table::setSchema(BaseObject *schema)
{
	QString prev_name=this->getName(true);
	BaseObject::setSchema(schema);
	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

void Table::setWithOIDs(bool value)
{
	with_oid=value;
}

void Table::setProtected(bool value)
{
	ObjectType obj_types[]={ OBJ_COLUMN, OBJ_CONSTRAINT,
													 OBJ_INDEX, OBJ_RULE, OBJ_TRIGGER };
	unsigned i;
	vector<TableObject *>::iterator itr, itr_end;
	vector<TableObject *> *list=nullptr;
	TableObject *tab_obj=nullptr;

	//Protected the table child objects
	for(i=0; i < 5; i++)
	{
		list=getObjectList(obj_types[i]);
		itr=list->begin();
		itr_end=list->end();

		while(itr!=itr_end)
		{

			tab_obj=(*itr);

			/* Relationship included object are always protected, so
			the protection state of this objects is not altered */
			if(!tab_obj->isAddedByRelationship())
				tab_obj->setProtected(value);
			itr++;
		}
	}

	//Protectes the table itself
	BaseGraphicObject::setProtected(value);
}

void Table::setCommentAttribute(TableObject *tab_obj)
{
	if(tab_obj && !tab_obj->getComment().isEmpty())
	{
		map<QString, QString> attribs;

		attribs[ParsersAttributes::DIF_SQL]="1";
		attribs[ParsersAttributes::SQL_OBJECT]=tab_obj->getSQLName();
		attribs[ParsersAttributes::COLUMN]=(tab_obj->getObjectType()==OBJ_COLUMN ? "1" : "");
		attribs[ParsersAttributes::CONSTRAINT]=(tab_obj->getObjectType()==OBJ_CONSTRAINT ? "1" : "");
		attribs[ParsersAttributes::TABLE]=this->getName(true);
		attribs[ParsersAttributes::NAME]=tab_obj->getName(true);
		attribs[ParsersAttributes::COMMENT]=tab_obj->getComment();

		SchemaParser::setIgnoreUnkownAttributes(true);
		attributes[ParsersAttributes::COLS_COMMENT]+=SchemaParser::getCodeDefinition(ParsersAttributes::COMMENT, attribs,
																																								 SchemaParser::SQL_DEFINITION);
		SchemaParser::setIgnoreUnkownAttributes(false);
	}
}

void Table::setColumnsAttribute(unsigned def_type)
{
	QString str_cols;
	unsigned i, count;

	//Concatena a definiação SQL das colunas
	count=columns.size();
	for(i=0; i < count; i++)
	{
		/* Do not generates the column code definition when it is not included by
		 relatoinship, in case of XML definition. */
		if((def_type==SchemaParser::SQL_DEFINITION && !columns[i]->isAddedByCopy())||
			 (def_type==SchemaParser::XML_DEFINITION &&	!columns[i]->isAddedByRelationship()))
		{
			str_cols+=columns[i]->getCodeDefinition(def_type);

			if(def_type==SchemaParser::SQL_DEFINITION)
				setCommentAttribute(columns[i]);
		}
	}

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		if(str_cols!="")
		{
			count=str_cols.size();
			if(str_cols[count-2]==',' || str_cols[count-2]=='\n')
				str_cols.remove(count-2,2);
		}
	}

	attributes[ParsersAttributes::COLUMNS]=str_cols;
}

void Table::setConstraintsAttribute(unsigned def_type)
{
	QString str_constr;
	unsigned i, count;
	bool inc_added_by_rel;
	Constraint *constr=nullptr;

	count=constraints.size();
	for(i=0; i < count; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);

		if((def_type==SchemaParser::SQL_DEFINITION &&
				(!constr->isReferRelationshipAddedColumn() || constr->getConstraintType()==ConstraintType::primary_key)) ||

			 (def_type==SchemaParser::XML_DEFINITION && !constr->isAddedByRelationship() &&
				((constr->getConstraintType()!=ConstraintType::primary_key && !constr->isReferRelationshipAddedColumn()) ||
				 (constr->getConstraintType()==ConstraintType::primary_key))))
		{
			inc_added_by_rel=(def_type==SchemaParser::SQL_DEFINITION);
			str_constr+=constr->getCodeDefinition(def_type,inc_added_by_rel);

			if(def_type==SchemaParser::SQL_DEFINITION)
				setCommentAttribute(constr);
		}
	}

	if(def_type==SchemaParser::SQL_DEFINITION)
	{
		if(str_constr!="")
		{
			count=str_constr.size();
			if(str_constr[count-2]==',' || str_constr[count-2]=='\n')
				str_constr.remove(count-2,2);
		}
	}

	attributes[ParsersAttributes::CONSTRAINTS]=str_constr;
}

void Table::setTriggersAttribute(unsigned def_type)
{
	QString str_trig;
	unsigned i, count;
	Trigger *trig=nullptr;

	count=triggers.size();
	for(i=0; i < count; i++)
	{
		trig=dynamic_cast<Trigger *>(triggers.at(i));

		/* Triggers that references columns added by relationship (special object)
		 do not have their code definition generated here. They are treated
		 in the database model code generation method */
		if((!trig->isReferRelationshipAddedColumn() &&
				def_type==SchemaParser::XML_DEFINITION) ||
			 def_type==SchemaParser::SQL_DEFINITION)
		{
			str_trig+=trig->getCodeDefinition(def_type);
		}
	}

	attributes[ParsersAttributes::TRIGGERS]=str_trig;
}

void Table::setIndexesAttribute(unsigned def_type)
{
	QString str_ind;
	unsigned i, count;
	Index *ind=nullptr;

	count=indexes.size();
	for(i=0; i < count; i++)
	{
		ind=dynamic_cast<Index *>(indexes[i]);

		/* Indexes that references columns added by relationship (special object)
		 do not have their code definition generated here. They are treated
		 in the database model code generation method */
		if((!ind->isAddedByRelationship() &&
				!ind->isReferRelationshipAddedColumn() &&
				def_type==SchemaParser::XML_DEFINITION) ||
			 def_type==SchemaParser::SQL_DEFINITION)
			str_ind+=ind->getCodeDefinition(def_type);
	}

	attributes[ParsersAttributes::INDEXES]=str_ind;
}

void Table::setRulesAttribute(unsigned def_type)
{
	QString str_rule;
	unsigned i, count;

	count=rules.size();
	for(i=0; i < count; i++)
	{
		if((def_type==SchemaParser::XML_DEFINITION) ||
			 def_type==SchemaParser::SQL_DEFINITION)
			str_rule+=rules[i]->getCodeDefinition(def_type);
	}

	attributes[ParsersAttributes::RULES]=str_rule;
}

vector<TableObject *> *Table::getObjectList(ObjectType obj_type)
{
	if(obj_type==OBJ_COLUMN)
		return(&columns);
	else if(obj_type==OBJ_CONSTRAINT)
		return(&constraints);
	else if(obj_type==OBJ_RULE)
		return(&rules);
	else if(obj_type==OBJ_TRIGGER)
		return(&triggers);
	else if(obj_type==OBJ_INDEX)
		return(&indexes);
	else
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

void Table::addObject(BaseObject *obj, int obj_idx)
{
	ObjectType obj_type;

	if(!obj)
		throw Exception(ERR_ASG_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	else
	{
		int idx;
		obj_type=obj->getObjectType();

		try
		{
			//Raises an error if already exists a object with the same name and type
			if(getObject(obj->getName(),obj_type,idx))
			{
				throw Exception(QString(Exception::getErrorMessage(ERR_ASG_DUPLIC_OBJECT))
												.arg(obj->getName(true))
												.arg(obj->getTypeName())
												.arg(this->getName(true))
												.arg(this->getTypeName()),
												ERR_ASG_DUPLIC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			//Raises an error if the user try to set the table as ancestor/copy of itself
			else if((obj_type==OBJ_TABLE || obj_type==BASE_TABLE) && obj==this)
				throw Exception(ERR_INV_INH_COPY_RELATIONSHIP,__PRETTY_FUNCTION__,__FILE__,__LINE__);

			switch(obj_type)
			{
				case OBJ_COLUMN:
				case OBJ_CONSTRAINT:
				case OBJ_TRIGGER:
				case OBJ_INDEX:
				case OBJ_RULE:
					TableObject *tab_obj;
					vector<TableObject *> *obj_list;

					tab_obj=dynamic_cast<TableObject *>(obj);

					//Sets the object parent table if there isn't one
					if(!tab_obj->getParentTable())
						tab_obj->setParentTable(this);
					//Raises an error if the parent table of the table object is different from table 'this'
					else if(tab_obj->getParentTable()!=this)
						throw Exception(ERR_ASG_OBJ_BELONGS_OTHER_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

					//Validates the object SQL code befor insert on table
					obj->getCodeDefinition(SchemaParser::SQL_DEFINITION);

					if(obj_type==OBJ_CONSTRAINT)
					{
						//Raises a error if the user try to add a second primary key on the table
						if(dynamic_cast<Constraint *>(tab_obj)->getConstraintType()==ConstraintType::primary_key &&
							 this->getPrimaryKey())
							throw Exception(ERR_ASG_EXISTING_PK_TABLE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
					}
					else if(obj_type==OBJ_TRIGGER)
						dynamic_cast<Trigger *>(tab_obj)->validateTrigger();

					obj_list=getObjectList(obj_type);

					//Adds the object to the table
					if(obj_idx < 0 || obj_idx >= static_cast<int>(obj_list->size()))
						obj_list->push_back(tab_obj);
					else
					{
						//If there is a object index specified inserts the object at the position
						if(obj_list->size() > 0)
							obj_list->insert((obj_list->begin() + obj_idx), tab_obj);
						else
							obj_list->push_back(tab_obj);
					}
				break;

				case OBJ_TABLE:
					Table *tab;
					tab=dynamic_cast<Table *>(obj);
					if(obj_idx < 0 || obj_idx >= static_cast<int>(ancestor_tables.size()))
						ancestor_tables.push_back(tab);
					else
						ancestor_tables.insert((ancestor_tables.begin() + obj_idx), tab);
				break;

				default:
					throw Exception(ERR_ASG_OBJECT_INV_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				break;
			}

		}
		catch(Exception &e)
		{
			if(e.getErrorType()==ERR_UNDEF_ATTRIB_VALUE)
				throw Exception(Exception::getErrorMessage(ERR_ASG_OBJ_INV_DEFINITION)
												.arg(Utf8String::create(obj->getName()))
												.arg(obj->getTypeName()),
												ERR_ASG_OBJ_INV_DEFINITION,__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
			else
				throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

void Table::addColumn(Column *col, int idx)
{
	try
	{
		addObject(col, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addTrigger(Trigger *trig, int idx)
{
	try
	{
		addObject(trig, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addIndex(Index *ind, int idx)
{
	try
	{
		addObject(ind, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addRule(Rule *reg, int idx_reg)
{
	try
	{
		addObject(reg, idx_reg);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addConstraint(Constraint *constr, int idx)
{
	try
	{
		addObject(constr, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::addAncestorTable(Table *tab, int idx)
{
	try
	{
		addObject(tab, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::setCopyTable(Table *tab)
{
	copy_table=tab;

	if(!copy_table)
		copy_op=CopyOptions(0,0);
}

void Table::setCopyTableOptions(CopyOptions like_op)
{
	if(copy_table)
		this->copy_op=like_op;
}

Table *Table::getCopyTable(void)
{
	return(copy_table);
}

CopyOptions Table::getCopyTableOptions(void)
{
	return(copy_op);
}

void Table::removeObject(BaseObject *obj)
{
	try
	{
		if(obj)
		{
			TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

			if(tab_obj)
				removeObject(getObjectIndex(tab_obj), obj->getObjectType());
			else
				removeObject(obj->getName(), OBJ_TABLE);
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeObject(const QString &name, ObjectType obj_type)
{
	int idx;

	//Gets the object index
	getObject(name,obj_type,idx);

	//Removes the object If it was found (idx >= 0)
	if(idx>=0)
		removeObject(static_cast<unsigned>(idx),obj_type);
}

void Table::removeObject(unsigned obj_idx, ObjectType obj_type)
{
	//Raises an error if the user try to remove a object with invalid type
	if(obj_type!=OBJ_COLUMN && obj_type!=OBJ_CONSTRAINT &&
		 obj_type!=OBJ_TRIGGER && obj_type!=OBJ_INDEX &&
		 obj_type!=OBJ_RULE && obj_type!=OBJ_TABLE)
		throw Exception(ERR_REM_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	else if(obj_type==OBJ_TABLE && obj_idx < ancestor_tables.size())
	{
		vector<Table *>::iterator itr;

		itr=ancestor_tables.begin() + obj_idx;
		ancestor_tables.erase(itr);
	}
	else if(obj_type!=OBJ_TABLE && obj_type!=BASE_TABLE)
	{
		vector<TableObject *> *obj_list=nullptr;
		vector<TableObject *>::iterator itr;

		obj_list=getObjectList(obj_type);

		//Raises an error if the object index is out of bound
		if(obj_idx >= obj_list->size())
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		if(obj_type!=OBJ_COLUMN)
		{
			itr=obj_list->begin() + obj_idx;
			(*itr)->setParentTable(nullptr);
			obj_list->erase(itr);
		}
		else
		{
			vector<TableObject *> refs;
			Column *column=nullptr;

			itr=obj_list->begin() + obj_idx;
			column=dynamic_cast<Column *>(*itr);

			//Gets the references to the column before the exclusion
			getColumnReferences(column, refs, true);

			//Case some trigger, constraint, index is referencing the column raises an error
			if(!refs.empty())
			{
				throw Exception(Exception::getErrorMessage(ERR_REM_INDIRECT_REFERENCE)
												.arg(Utf8String::create(column->getName()))
												.arg(column->getTypeName())
												.arg(Utf8String::create(refs[0]->getName()))
						.arg(refs[0]->getTypeName())
						.arg(Utf8String::create(this->getName(true)))
						.arg(this->getTypeName()),
						ERR_REM_INDIRECT_REFERENCE,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			}

			column->setParentTable(nullptr);
			columns.erase(itr);
		}
	}
}

void Table::removeColumn(const QString &name)
{
	try
	{
		removeObject(name,OBJ_COLUMN);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeColumn(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_COLUMN);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeTrigger(const QString &name)
{
	try
	{
		removeObject(name,OBJ_TRIGGER);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeTrigger(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_TRIGGER);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeIndex(const QString &name)
{
	try
	{
		removeObject(name,OBJ_INDEX);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeIndex(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_INDEX);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeRule(const QString &name)
{
	try
	{
		removeObject(name,OBJ_RULE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeRule(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_RULE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeConstraint(const QString &name)
{
	try
	{
		removeObject(name,OBJ_CONSTRAINT);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeConstraint(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_CONSTRAINT);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeAncestorTable(const QString &name)
{
	try
	{
		removeObject(name,OBJ_TABLE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void Table::removeAncestorTable(unsigned idx)
{
	try
	{
		removeObject(idx,OBJ_TABLE);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

int Table::getObjectIndex(const QString &name, ObjectType obj_type)
{
	int idx;
	getObject(name, obj_type, idx);
	return(idx);
}

int Table::getObjectIndex(BaseObject *obj)
{
	TableObject *tab_obj=dynamic_cast<TableObject *>(obj);

	if(!obj)
		return(-1);
	else
	{
		vector<TableObject *> *obj_list = this->getObjectList(obj->getObjectType());
		vector<TableObject *>::iterator itr, itr_end;
		bool found=false;

		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end && !found)
		{
			found=((*itr)==tab_obj);
			if(!found) itr++;
		}

		if(found)
			return(itr-obj_list->begin());
		else
			return(-1);
	}
}

BaseObject *Table::getObject(const QString &name, ObjectType obj_type)
{
	int idx;
	return(getObject(name, obj_type, idx));
}

BaseObject *Table::getObject(const QString &name, ObjectType obj_type, int &obj_idx)
{
	BaseObject *object=nullptr;
	bool found=false, format=false;

	//Checks if the name contains ", if so, the search will consider formatted names
	format=name.contains("\"");

	if(TableObject::isTableObject(obj_type))
	{
		vector<TableObject *>::iterator itr, itr_end;
		vector<TableObject *> *obj_list=nullptr;
		QString aux_name=name;

		obj_list=getObjectList(obj_type);
		itr=obj_list->begin();
		itr_end=obj_list->end();

		while(itr!=itr_end)
		{
			found=((*itr)->getName(format)==aux_name);
			if(!found) itr++;
			else break;
		}

		if(found)
		{
			obj_idx=(itr-obj_list->begin());
			object=(*itr);
		}
		else obj_idx=-1;
	}
	else if(obj_type==OBJ_TABLE)
	{
		vector<Table *>::iterator itr_tab, itr_end_tab;
		QString aux_name=BaseObject::formatName(name);

		itr_tab=ancestor_tables.begin();
		itr_end_tab=ancestor_tables.end();

		while(itr_tab!=itr_end_tab)
		{
			/* Unlike other object types, tables are always compared with the FORMATTED NAME
			because they must be 'schema-qualified' preventing a table of the same name
			but different schemas are confused */
			found=((*itr_tab)->getName(true)==aux_name);
			if(!found) itr_tab++;
			else break;
		}

		if(found)
		{
			obj_idx=(itr_tab-ancestor_tables.begin());
			object=(*itr_tab);
		}
		else obj_idx=-1;
	}
	else
		throw Exception(ERR_OBT_OBJ_INVALID_TYPE,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return(object);
}

BaseObject *Table::getObject(unsigned obj_idx, ObjectType obj_type)
{
	vector<TableObject *> *obj_list=nullptr;

	if(obj_type==OBJ_TABLE)
	{
		//Raises an error if the object index is out of bound
		if(obj_idx >= ancestor_tables.size())
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		return(ancestor_tables[obj_idx]);
	}
	else
	{
		obj_list=getObjectList(obj_type);
		if(obj_idx < obj_list->size())
			return(obj_list->at(obj_idx));
		else
			//Raises an error if the object index is out of bound
			throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
	}
}

Table *Table::getAncestorTable(const QString &name)
{
	int idx;
	return(dynamic_cast<Table *>(getObject(name,OBJ_TABLE,idx)));
}

Table *Table::getAncestorTable(unsigned idx)
{
	return(dynamic_cast<Table *>(getObject(idx,OBJ_TABLE)));
}

Column *Table::getColumn(const QString &name, bool ref_old_name)
{
	if(!ref_old_name)
	{
		int idx;
		return(dynamic_cast<Column *>(getObject(name,OBJ_COLUMN,idx)));
	}
	else
	{
		Column *column=nullptr;
		vector<TableObject *>::iterator itr, itr_end;
		bool found=false, format=false;

		format=name.contains("\"");
		itr=columns.begin();
		itr_end=columns.end();

		//Search the column referencing the old name
		while(itr!=itr_end && !found)
		{
			column=dynamic_cast<Column *>(*itr);
			itr++;
			found=(name!="" && column->getOldName(format)==name);
		}

		if(!found) column=nullptr;
		return(column);
	}
}

Column *Table::getColumn(unsigned idx)
{
	return(dynamic_cast<Column *>(getObject(idx,OBJ_COLUMN)));
}

Trigger *Table::getTrigger(const QString &name)
{
	int idx;
	return(dynamic_cast<Trigger *>(getObject(name,OBJ_TRIGGER,idx)));
}

Trigger *Table::getTrigger(unsigned idx)
{
	return(dynamic_cast<Trigger *>(getObject(idx,OBJ_TRIGGER)));
}

Constraint *Table::getConstraint(const QString &name)
{
	int idx;
	return(dynamic_cast<Constraint *>(getObject(name,OBJ_CONSTRAINT,idx)));
}

Constraint *Table::getConstraint(unsigned idx)
{
	return(dynamic_cast<Constraint *>(getObject(idx,OBJ_CONSTRAINT)));
}

Index *Table::getIndex(const QString &name)
{
	int idx;
	return(dynamic_cast<Index *>(getObject(name,OBJ_INDEX,idx)));
}

Index *Table::getIndex(unsigned idx)
{
	return(dynamic_cast<Index *>(getObject(idx,OBJ_INDEX)));
}

Rule *Table::getRule(const QString &name)
{
	int idx;
	return(dynamic_cast<Rule *>(getObject(name,OBJ_RULE,idx)));
}

Rule *Table::getRule(unsigned idx)
{
	return(dynamic_cast<Rule *>(getObject(idx,OBJ_RULE)));
}

unsigned Table::getColumnCount(void)
{
	return(columns.size());
}

unsigned Table::getTriggerCount(void)
{
	return(triggers.size());
}

unsigned Table::getConstraintCount(void)
{
	return(constraints.size());
}

unsigned Table::getIndexCount(void)
{
	return(indexes.size());
}

unsigned Table::getRuleCount(void)
{
	return(rules.size());
}

unsigned Table::getAncestorTableCount(void)
{
	return(ancestor_tables.size());
}

unsigned Table::getObjectCount(ObjectType obj_type, bool inc_added_by_rel)
{
	if(TableObject::isTableObject(obj_type) || obj_type==OBJ_TABLE)
	{
		if(obj_type==OBJ_TABLE)
		{
			return(ancestor_tables.size());
		}
		else
		{
			vector<TableObject *> *list=nullptr;
			list=getObjectList(obj_type);

			if(!inc_added_by_rel)
			{
				vector<TableObject *>::iterator itr, itr_end;
				unsigned count=0;

				itr=list->begin();
				itr_end=list->end();
				while(itr!=itr_end)
				{
					if(!(*itr)->isAddedByRelationship()) count++;
					itr++;
				}

				return(count);
			}
			else
				return(list->size());
		}
	}
	else
		throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
}

Constraint *Table::getPrimaryKey(void)
{
	unsigned count,i;
	Constraint *pk=nullptr, *constr=nullptr;

	count=constraints.size();
	for(i=0; i < count && !pk; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);
		pk=(constr->getConstraintType()==ConstraintType::primary_key ? constr : nullptr);
	}

	return(pk);
}

void Table::getForeignKeys(vector<Constraint *> &fks, bool inc_added_by_rel, Table *ref_table)
{
	unsigned count,i;
	Constraint *constr=nullptr;

	count=constraints.size();
	for(i=0; i < count; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);

		if(constr->getConstraintType()==ConstraintType::foreign_key &&
			 (!ref_table || (ref_table && constr->getReferencedTable()==ref_table)) &&
			 (!constr->isAddedByLinking() ||
				(constr->isAddedByLinking() && inc_added_by_rel)))
			fks.push_back(constr);
	}
}

bool Table::isWithOIDs(void)
{
	return(with_oid);
}

bool Table::isReferTableOnForeignKey(Table *ref_tab)
{
	unsigned count,i;
	Constraint *constr=nullptr;
	bool found=false;

	count=constraints.size();
	for(i=0; i < count && !found; i++)
	{
		constr=dynamic_cast<Constraint *>(constraints[i]);
		found=(constr->getConstraintType()==ConstraintType::foreign_key &&
					 !constr->isAddedByLinking() &&
					 constr->getReferencedTable() == ref_tab);
	}

	return(found);
}

bool Table::isConstraintRefColumn(Column *column, ConstraintType constr_type)
{
	bool found=false;
	vector<TableObject *>::iterator itr, itr_end;
	Constraint *constr=nullptr;

	if(column)
	{
		itr=constraints.begin();
		itr_end=constraints.end();
		while(itr!=itr_end && !found)
		{
			constr=dynamic_cast<Constraint *>(*itr);
			itr++;
			found=(constr->getConstraintType()==constr_type &&
						 constr->isColumnReferenced(column));
		}
	}

	return(found);
}

QString Table::getCodeDefinition(unsigned def_type)
{
	attributes[ParsersAttributes::OIDS]=(with_oid ? "1" : "");
	attributes[ParsersAttributes::COPY_TABLE]="";

	if(def_type==SchemaParser::SQL_DEFINITION && copy_table)
		attributes[ParsersAttributes::COPY_TABLE]=copy_table->getName(true) + copy_op.getSQLDefinition();

	(copy_table ? copy_table->getName(true) : "");

	setColumnsAttribute(def_type);
	setConstraintsAttribute(def_type);
	setTriggersAttribute(def_type);
	setIndexesAttribute(def_type);
	setRulesAttribute(def_type);

	if(def_type==SchemaParser::XML_DEFINITION)
		setPositionAttribute();

	return(BaseObject::__getCodeDefinition(def_type));
}

void Table::operator = (Table &tab)
{
	QString prev_name = this->getName(true);

	(*dynamic_cast<BaseGraphicObject *>(this))=dynamic_cast<BaseGraphicObject &>(tab);
	this->with_oid=tab.with_oid;
	setProtected(tab.is_protected);

	PgSQLType::renameUserType(prev_name, this, this->getName(true));
}

bool Table::isReferRelationshipAddedObject(void)
{
	vector<TableObject *>::iterator itr, itr_end;
	ObjectType types[]={ OBJ_COLUMN, OBJ_CONSTRAINT };
	bool found=false;

	for(unsigned i=0; i < 2 && !found; i++)
	{
		itr=getObjectList(types[i])->begin();
		itr_end=getObjectList(types[i])->end();

		while(itr!=itr_end && !found)
		{
			found=(*itr)->isAddedByRelationship();
			itr++;
		}
	}

	return(found);
}

void Table::swapObjectsIndexes(ObjectType obj_type, unsigned idx1, unsigned idx2)
{
	vector<TableObject *> *obj_list=nullptr;
	vector<TableObject *>::iterator itr1, itr2;
	TableObject *aux_obj=nullptr;

	try
	{
		if(idx1!=idx2)
		{
			obj_list=getObjectList(obj_type);

			//Raises an error if both index is out of list bounds
			if(idx1 >= obj_list->size() && idx2 >= obj_list->size())
				throw Exception(ERR_REF_OBJ_INV_INDEX,__PRETTY_FUNCTION__,__FILE__,__LINE__);
			//If the idx1 is out of bound inserts the element idx2 at the list's begin
			else if(idx1 >= obj_list->size())
			{
				itr2=obj_list->begin() + idx2;
				aux_obj=(*itr2);
				obj_list->erase(itr2);
				obj_list->insert(obj_list->begin(), aux_obj);
			}
			//If the idx2 is out of bound inserts the element idx1 on the list's end
			else if(idx2 >= obj_list->size())
			{
				itr1=obj_list->begin() + idx1;
				aux_obj=(*itr1);
				obj_list->erase(itr1);
				obj_list->push_back(aux_obj);
			}
			else
			{
				aux_obj=obj_list->at(idx1);
				itr1=obj_list->begin() + idx1;
				itr2=obj_list->begin() + idx2;

				(*itr1)=(*itr2);
				(*itr2)=aux_obj;
			}
		}
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Table::moveObjectToIndex(TableObject *tab_obj, unsigned idx)
{
	unsigned curr_idx;

	try
	{
		if(!tab_obj)
			throw Exception(ERR_OPR_NOT_ALOC_OBJECT,__PRETTY_FUNCTION__,__FILE__,__LINE__);

		curr_idx=getObjectIndex(tab_obj);

		if(curr_idx!=idx)
			swapObjectsIndexes(tab_obj->getObjectType(), curr_idx, idx);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Table::getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode)
{
	if(column && !column->isAddedByRelationship())
	{
		unsigned count, i;
		IndexElement elem;
		Column *col=nullptr, *col1=nullptr;
		vector<TableObject *>::iterator itr, itr_end;
		bool found=false;
		Index *ind=nullptr;
		Constraint *constr=nullptr;
		Trigger *trig=nullptr;

		itr=indexes.begin();
		itr_end=indexes.end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
		{
			ind=dynamic_cast<Index *>(*itr);
			itr++;

			count=ind->getIndexElementCount();
			for(i=0; i < count  && (!exclusion_mode || (exclusion_mode && !found)); i++)
			{
				elem=ind->getIndexElement(i);
				col=elem.getColumn();
				if(col && col==column)
				{
					found=true;
					refs.push_back(ind);
				}
			}
		}

		itr=constraints.begin();
		itr_end=constraints.end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
		{
			constr=dynamic_cast<Constraint *>(*itr);
			itr++;

			col=constr->getColumn(column->getName(),true);
			col1=constr->getColumn(column->getName(),false);

			if((col && col==column) || (col1 && col1==column))
			{
				found=true;
				refs.push_back(constr);
			}
		}

		itr=triggers.begin();
		itr_end=triggers.end();

		while(itr!=itr_end && (!exclusion_mode || (exclusion_mode && !found)))
		{
			trig=dynamic_cast<Trigger *>(*itr);
			itr++;

			count=trig->getColumnCount();
			for(i=0; i < count && (!exclusion_mode || (exclusion_mode && !found)); i++)
			{
				if(trig->getColumn(i)==column)
				{
					found=true;
					refs.push_back(trig);
				}
			}
		}
	}
}

vector<BaseObject *> Table::getObjects(void)
{
	vector<BaseObject *> list;

	list.assign(columns.begin(), columns.end());
	list.insert(list.end(), constraints.begin(), constraints.end());
	list.insert(list.end(), triggers.begin(), triggers.end());
	list.insert(list.end(), rules.begin(), rules.end());
	list.insert(list.end(), indexes.begin(), indexes.end());

	return(list);
}
