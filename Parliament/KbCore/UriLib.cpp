// Parliament is licensed under the BSD License from the Open Source
// Initiative, http://www.opensource.org/licenses/bsd-license.php
//
// Copyright (c) 2001-2009, BBN Technologies, Inc.
// All rights reserved.

#include "parliament/UriLib.h"
#include "parliament/Exceptions.h"
#include "parliament/KbInstance.h"
#include "parliament/UnicodeIterator.h"

#include <mutex>

namespace pmnt = ::bbn::parliament;

using LockGuard = ::std::lock_guard< ::std::recursive_mutex >;

static ::std::recursive_mutex g_lazyRsrcMutableMemberMutex;

pmnt::LazyRsrc::LazyRsrc(KbInstance* pKB, const char*const pUri) :
	m_pKB(pKB),
	m_pUri(pUri),
	m_uri(),
	m_id(k_nullRsrcId)
{
}

pmnt::ResourceId pmnt::LazyRsrc::id() const
{
	if (m_pKB == nullptr)
	{
		throw Exception{"Call to id() for a UriLib initialized with a null KbInstance"};
	}

	LockGuard guard{g_lazyRsrcMutableMemberMutex};
	if (m_id == k_nullRsrcId)
	{
		m_id = m_pKB->uriToRsrcId(str(), false, true);
	}
	return m_id;
}

const pmnt::RsrcString& pmnt::LazyRsrc::str() const
{
	LockGuard guard{g_lazyRsrcMutableMemberMutex};
	if (m_uri.empty())
	{
		m_uri = strNoCache();
	}
	return m_uri;
}

pmnt::RsrcString pmnt::LazyRsrc::strNoCache() const
{
	return convertToRsrcChar(m_pUri);
}

pmnt::UriLib::UriLib(KbInstance* pKB) :
	m_gisGMLLiteral(pKB, "http://www.opengis.net/rdf#GMLLiteral"),
	m_gisWKTLiteral(pKB, "http://www.opengis.net/rdf#WKTLiteral"),
	m_owlAnnotationProperty(pKB, "http://www.w3.org/2002/07/owl#AnnotationProperty"),
	m_owlAsymmetricProperty(pKB, "http://www.w3.org/2002/07/owl#AsymmetricProperty"),
	m_owlCard(pKB, "http://www.w3.org/2002/07/owl#cardinality"),
	m_owlClass(pKB, "http://www.w3.org/2002/07/owl#Class"),
	m_owlDatatypeProp(pKB, "http://www.w3.org/2002/07/owl#DatatypeProperty"),
	m_owlDeprecatedProperty(pKB, "http://www.w3.org/2002/07/owl#DeprecatedProperty"),
	m_owlDifferentFrom(pKB, "http://www.w3.org/2002/07/owl#differentFrom"),
	m_owlEquivalentClass(pKB, "http://www.w3.org/2002/07/owl#equivalentClass"),
	m_owlEquivalentProp(pKB, "http://www.w3.org/2002/07/owl#equivalentProperty"),
	m_owlFuncProp(pKB, "http://www.w3.org/2002/07/owl#FunctionalProperty"),
	m_owlHasValue(pKB, "http://www.w3.org/2002/07/owl#hasValue"),
	m_owlInverseOf(pKB, "http://www.w3.org/2002/07/owl#inverseOf"),
	m_owlInvFuncProp(pKB, "http://www.w3.org/2002/07/owl#InverseFunctionalProperty"),
	m_owlIrreflexiveProperty(pKB, "http://www.w3.org/2002/07/owl#IrreflexiveProperty"),
	m_owlMaxCard(pKB, "http://www.w3.org/2002/07/owl#maxCardinality"),
	m_owlMinCard(pKB, "http://www.w3.org/2002/07/owl#minCardinality"),
	m_owlObjectProp(pKB, "http://www.w3.org/2002/07/owl#ObjectProperty"),
	m_owlOnProp(pKB, "http://www.w3.org/2002/07/owl#onProperty"),
	m_owlReflexiveProperty(pKB, "http://www.w3.org/2002/07/owl#ReflexiveProperty"),
	m_owlRestriction(pKB, "http://www.w3.org/2002/07/owl#Restriction"),
	m_owlSameAs(pKB, "http://www.w3.org/2002/07/owl#sameAs"),
	m_owlSymmetricProp(pKB, "http://www.w3.org/2002/07/owl#SymmetricProperty"),
	m_owlThing(pKB, "http://www.w3.org/2002/07/owl#Thing"),
	m_owlTransitiveProp(pKB, "http://www.w3.org/2002/07/owl#TransitiveProperty"),
	m_parDirectSubClassOf(pKB, "http://parliament.semwebcentral.org/parliament#directSubClassOf"),
	m_parDirectType(pKB, "http://parliament.semwebcentral.org/parliament#directType"),
	m_parHasStatementName(pKB, "http://parliament.semwebcentral.org/parliament#hasStatementName"),
	m_parSwrlTrigger(pKB, "http://parliament.semwebcentral.org/parliament#swrlTrigger"),
	m_ptIntervalLiteral(pKB, "http://bbn.com/ParliamentTime#intervalLiteral"),
	m_rdfHTML(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#HTML"),
	m_rdfLangString(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#langString"),
	m_rdfListFirst(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#first"),
	m_rdfListNil(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#nil"),
	m_rdfListRest(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#rest"),
	m_rdfObject(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#object"),
	m_rdfPlainLiteral(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral"),
	m_rdfPredicate(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#predicate"),
	m_rdfProperty(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#Property"),
	m_rdfsClass(pKB, "http://www.w3.org/2000/01/rdf-schema#Class"),
	m_rdfsComment(pKB, "http://www.w3.org/2000/01/rdf-schema#comment"),
	m_rdfsDatatype(pKB, "http://www.w3.org/2000/01/rdf-schema#Datatype"),
	m_rdfsDomain(pKB, "http://www.w3.org/2000/01/rdf-schema#domain"),
	m_rdfsLabel(pKB, "http://www.w3.org/2000/01/rdf-schema#label"),
	m_rdfsRange(pKB, "http://www.w3.org/2000/01/rdf-schema#range"),
	m_rdfsResource(pKB, "http://www.w3.org/2000/01/rdf-schema#Resource"),
	m_rdfsSubClassOf(pKB, "http://www.w3.org/2000/01/rdf-schema#subClassOf"),
	m_rdfsSubPropertyOf(pKB, "http://www.w3.org/2000/01/rdf-schema#subPropertyOf"),
	m_rdfStatement(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#Statement"),
	m_rdfSubject(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#subject"),
	m_rdfType(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#type"),
	m_rdfXMLLiteral(pKB, "http://www.w3.org/1999/02/22-rdf-syntax-ns#XMLLiteral"),
	m_ruleDomain(pKB, "http://www.bbn.com/2005/09/rules/rdfs#domain"),
	m_ruleEquivalentClass(pKB, "http://www.bbn.com/2005/09/rules/owl#equivalentClass"),
	m_ruleEquivalentProp(pKB, "http://www.bbn.com/2005/09/rules/owl#equivalentProperty"),
	m_ruleFuncProp(pKB, "http://www.bbn.com/2005/09/rules/owl#functionalProperty"),
	m_ruleInverseProp(pKB, "http://www.bbn.com/2005/09/rules/rdfs#inverseProperty"),
	m_ruleInvFuncProp(pKB, "http://www.bbn.com/2005/09/rules/owl#inverseFunctionalProperty"),
	m_ruleRange(pKB, "http://www.bbn.com/2005/09/rules/rdfs#range"),
	m_ruleSelfSubclass(pKB, "http://www.bbn.com/2005/09/rules/rdfs#selfSubclass"),
	m_ruleSelfSubprop(pKB, "http://www.bbn.com/2005/09/rules/rdfs#selfSubprop"),
	m_ruleSubclass(pKB, "http://www.bbn.com/2005/09/rules/rdfs#subclass"),
	m_ruleSubproperty(pKB, "http://www.bbn.com/2005/09/rules/rdfs#subproperty"),
	m_ruleSymmetricProp(pKB, "http://www.bbn.com/2005/09/rules/rdfs#symmetricProperty"),
	m_ruleTransitiveProp(pKB, "http://www.bbn.com/2005/09/rules/owl#transitiveProperty"),
	m_swrlArgument1(pKB, "http://www.w3.org/2003/11/swrl#argument1"),
	m_swrlArgument2(pKB, "http://www.w3.org/2003/11/swrl#argument2"),
	m_swrlArguments(pKB, "http://www.w3.org/2003/11/swrl#arguments"),
	m_swrlbAbs(pKB, "http://www.w3.org/2003/11/swrlb#abs"),
	m_swrlbAdd(pKB, "http://www.w3.org/2003/11/swrlb#add"),
	m_swrlbAddDayTimeDurations(pKB, "http://www.w3.org/2003/11/swrlb#addDayTimeDurations"),
	m_swrlbAddDayTimeDurationToDate(pKB, "http://www.w3.org/2003/11/swrlb#addDayTimeDurationToDate"),
	m_swrlbAddDayTimeDurationToDateTime(pKB, "http://www.w3.org/2003/11/swrlb#addDayTimeDurationToDateTime"),
	m_swrlbAddDayTimeDurationToTime(pKB, "http://www.w3.org/2003/11/swrlb#addDayTimeDurationToTime"),
	m_swrlbAddYearMonthDurations(pKB, "http://www.w3.org/2003/11/swrlb#addYearMonthDurations"),
	m_swrlbAddYearMonthDurationToDate(pKB, "http://www.w3.org/2003/11/swrlb#addYearMonthDurationToDate"),
	m_swrlbAddYearMonthDurationToDateTime(pKB, "http://www.w3.org/2003/11/swrlb#addYearMonthDurationToDateTime"),
	m_swrlbAnyURI(pKB, "http://www.w3.org/2003/11/swrlb#anyURI"),
	m_swrlbBooleanNot(pKB, "http://www.w3.org/2003/11/swrlb#booleanNot"),
	m_swrlbCeiling(pKB, "http://www.w3.org/2003/11/swrlb#ceiling"),
	m_swrlbContains(pKB, "http://www.w3.org/2003/11/swrlb#contains"),
	m_swrlbContainsIgnoreCase(pKB, "http://www.w3.org/2003/11/swrlb#containsIgnoreCase"),
	m_swrlbCos(pKB, "http://www.w3.org/2003/11/swrlb#cos"),
	m_swrlbDate(pKB, "http://www.w3.org/2003/11/swrlb#date"),
	m_swrlbDateTime(pKB, "http://www.w3.org/2003/11/swrlb#dateTime"),
	m_swrlbDayTimeDuration(pKB, "http://www.w3.org/2003/11/swrlb#dayTimeDuration"),
	m_swrlbDivide(pKB, "http://www.w3.org/2003/11/swrlb#divide"),
	m_swrlbDivideDayTimeDuration(pKB, "http://www.w3.org/2003/11/swrlb#divideDayTimeDuration"),
	m_swrlbDivideYearMonthDurations(pKB, "http://www.w3.org/2003/11/swrlb#divideYearMonthDurations"),
	m_swrlbEmpty(pKB, "http://www.w3.org/2003/11/swrlb#empty"),
	m_swrlbEndsWith(pKB, "http://www.w3.org/2003/11/swrlb#endsWith"),
	m_swrlbEqual(pKB, "http://www.w3.org/2003/11/swrlb#equal"),
	m_swrlbFirst(pKB, "http://www.w3.org/2003/11/swrlb#first"),
	m_swrlbFloor(pKB, "http://www.w3.org/2003/11/swrlb#floor"),
	m_swrlbGreaterThan(pKB, "http://www.w3.org/2003/11/swrlb#greaterThan"),
	m_swrlbGreaterThanOrEqual(pKB, "http://www.w3.org/2003/11/swrlb#greaterThanOrEqual"),
	m_swrlbIntegerDivide(pKB, "http://www.w3.org/2003/11/swrlb#integerDivide"),
	m_swrlbLength(pKB, "http://www.w3.org/2003/11/swrlb#length"),
	m_swrlbLessThan(pKB, "http://www.w3.org/2003/11/swrlb#lessThan"),
	m_swrlbLessThanOrEqual(pKB, "http://www.w3.org/2003/11/swrlb#lessThanOrEqual"),
	m_swrlbListConcat(pKB, "http://www.w3.org/2003/11/swrlb#listConcat"),
	m_swrlbListIntersection(pKB, "http://www.w3.org/2003/11/swrlb#listIntersection"),
	m_swrlbListSubtraction(pKB, "http://www.w3.org/2003/11/swrlb#listSubtraction"),
	m_swrlbLowerCase(pKB, "http://www.w3.org/2003/11/swrlb#lowerCase"),
	m_swrlbMatches(pKB, "http://www.w3.org/2003/11/swrlb#matches"),
	m_swrlbMember(pKB, "http://www.w3.org/2003/11/swrlb#member"),
	m_swrlbMod(pKB, "http://www.w3.org/2003/11/swrlb#mod"),
	m_swrlbMultiply(pKB, "http://www.w3.org/2003/11/swrlb#multiply"),
	m_swrlbMultiplyDayTimeDurations(pKB, "http://www.w3.org/2003/11/swrlb#multiplyDayTimeDurations"),
	m_swrlbMultiplyYearMonthDuration(pKB, "http://www.w3.org/2003/11/swrlb#multiplyYearMonthDuration"),
	m_swrlbNormalizeSpace(pKB, "http://www.w3.org/2003/11/swrlb#normalizeSpace"),
	m_swrlbNotEqual(pKB, "http://www.w3.org/2003/11/swrlb#notEqual"),
	m_swrlBody(pKB, "http://www.w3.org/2003/11/swrl#body"),
	m_swrlbPow(pKB, "http://www.w3.org/2003/11/swrlb#pow"),
	m_swrlbReplace(pKB, "http://www.w3.org/2003/11/swrlb#replace"),
	m_swrlbResolveURI(pKB, "http://www.w3.org/2003/11/swrlb#resolveURI"),
	m_swrlbRest(pKB, "http://www.w3.org/2003/11/swrlb#rest"),
	m_swrlbRound(pKB, "http://www.w3.org/2003/11/swrlb#round"),
	m_swrlbRoundHalfToEven(pKB, "http://www.w3.org/2003/11/swrlb#roundHalfToEven"),
	m_swrlbSin(pKB, "http://www.w3.org/2003/11/swrlb#sin"),
	m_swrlbStartsWith(pKB, "http://www.w3.org/2003/11/swrlb#startsWith"),
	m_swrlbStringConcat(pKB, "http://www.w3.org/2003/11/swrlb#stringConcat"),
	m_swrlbStringEqualIgnoreCase(pKB, "http://www.w3.org/2003/11/swrlb#stringEqualIgnoreCase"),
	m_swrlbStringLength(pKB, "http://www.w3.org/2003/11/swrlb#stringLength"),
	m_swrlbSublist(pKB, "http://www.w3.org/2003/11/swrlb#sublist"),
	m_swrlbSubstring(pKB, "http://www.w3.org/2003/11/swrlb#substring"),
	m_swrlbSubstringAfter(pKB, "http://www.w3.org/2003/11/swrlb#substringAfter"),
	m_swrlbSubstringBefore(pKB, "http://www.w3.org/2003/11/swrlb#substringBefore"),
	m_swrlbSubtract(pKB, "http://www.w3.org/2003/11/swrlb#subtract"),
	m_swrlbSubtractDates(pKB, "http://www.w3.org/2003/11/swrlb#subtractDates"),
	m_swrlbSubtractDateTimesYieldingDayTimeDuration(pKB, "http://www.w3.org/2003/11/swrlb#subtractDateTimesYieldingDayTimeDuration"),
	m_swrlbSubtractDateTimesYieldingYearMonthDuration(pKB, "http://www.w3.org/2003/11/swrlb#subtractDateTimesYieldingYearMonthDuration"),
	m_swrlbSubtractDayTimeDurationFromDate(pKB, "http://www.w3.org/2003/11/swrlb#subtractDayTimeDurationFromDate"),
	m_swrlbSubtractDayTimeDurationFromDateTime(pKB, "http://www.w3.org/2003/11/swrlb#subtractDayTimeDurationFromDateTime"),
	m_swrlbSubtractDayTimeDurationFromTime(pKB, "http://www.w3.org/2003/11/swrlb#subtractDayTimeDurationFromTime"),
	m_swrlbSubtractDayTimeDurations(pKB, "http://www.w3.org/2003/11/swrlb#subtractDayTimeDurations"),
	m_swrlbSubtractTimes(pKB, "http://www.w3.org/2003/11/swrlb#subtractTimes"),
	m_swrlbSubtractYearMonthDurationFromDate(pKB, "http://www.w3.org/2003/11/swrlb#subtractYearMonthDurationFromDate"),
	m_swrlbSubtractYearMonthDurationFromDateTime(pKB, "http://www.w3.org/2003/11/swrlb#subtractYearMonthDurationFromDateTime"),
	m_swrlbSubtractYearMonthDurations(pKB, "http://www.w3.org/2003/11/swrlb#subtractYearMonthDurations"),
	m_swrlbTan(pKB, "http://www.w3.org/2003/11/swrlb#tan"),
	m_swrlbTime(pKB, "http://www.w3.org/2003/11/swrlb#time"),
	m_swrlbTokenize(pKB, "http://www.w3.org/2003/11/swrlb#tokenize"),
	m_swrlbTranslate(pKB, "http://www.w3.org/2003/11/swrlb#translate"),
	m_swrlBuiltin(pKB, "http://www.w3.org/2003/11/swrl#builtin"),
	m_swrlBuiltinAtom(pKB, "http://www.w3.org/2003/11/swrl#BuiltinAtom"),
	m_swrlbUnaryMinus(pKB, "http://www.w3.org/2003/11/swrlb#unaryMinus"),
	m_swrlbUnaryPlus(pKB, "http://www.w3.org/2003/11/swrlb#unaryPlus"),
	m_swrlbUpperCase(pKB, "http://www.w3.org/2003/11/swrlb#upperCase"),
	m_swrlbYearMonthDuration(pKB, "http://www.w3.org/2003/11/swrlb#yearMonthDuration"),
	m_swrlClassAtom(pKB, "http://www.w3.org/2003/11/swrl#ClassAtom"),
	m_swrlClassPredicate(pKB, "http://www.w3.org/2003/11/swrl#classPredicate"),
	m_swrlDataRange(pKB, "http://www.w3.org/2003/11/swrl#dataRange"),
	m_swrlDataRangeAtom(pKB, "http://www.w3.org/2003/11/swrl#DataRangeAtom"),
	m_swrlDatavaluedPropertyAtom(pKB, "http://www.w3.org/2003/11/swrl#DatavaluedPropertyAtom"),
	m_swrlDifferentIndividualsAtom(pKB, "http://www.w3.org/2003/11/swrl#DifferentIndividualsAtom"),
	m_swrlHead(pKB, "http://www.w3.org/2003/11/swrl#head"),
	m_swrlImp(pKB, "http://www.w3.org/2003/11/swrl#Imp"),
	m_swrlIndividualPropertyAtom(pKB, "http://www.w3.org/2003/11/swrl#IndividualPropertyAtom"),
	m_swrlpropertyPred(pKB, "http://www.w3.org/2003/11/swrl#propertyPredicate"),
	m_swrlSameIndividualAtom(pKB, "http://www.w3.org/2003/11/swrl#SameIndividualsAtom"),
	m_swrlVariable(pKB, "http://www.w3.org/2003/11/swrl#Variable"),
	m_xsdAnyURI(pKB, "http://www.w3.org/2001/XMLSchema#anyURI"),
	m_xsdBase64Binary(pKB, "http://www.w3.org/2001/XMLSchema#base64Binary"),
	m_xsdBoolean(pKB, "http://www.w3.org/2001/XMLSchema#boolean"),
	m_xsdByte(pKB, "http://www.w3.org/2001/XMLSchema#byte"),
	m_xsdDate(pKB, "http://www.w3.org/2001/XMLSchema#date"),
	m_xsdDateTime(pKB, "http://www.w3.org/2001/XMLSchema#dateTime"),
	m_xsdDateTimeStamp(pKB, "http://www.w3.org/2001/XMLSchema#dateTimeStamp"),
	m_xsdDayTimeDuration(pKB, "http://www.w3.org/2001/XMLSchema#dayTimeDuration"),
	m_xsdDecimal(pKB, "http://www.w3.org/2001/XMLSchema#decimal"),
	m_xsdDouble(pKB, "http://www.w3.org/2001/XMLSchema#double"),
	m_xsdDuration(pKB, "http://www.w3.org/2001/XMLSchema#duration"),
	m_xsdFloat(pKB, "http://www.w3.org/2001/XMLSchema#float"),
	m_xsdGDay(pKB, "http://www.w3.org/2001/XMLSchema#gDay"),
	m_xsdGMonth(pKB, "http://www.w3.org/2001/XMLSchema#gMonth"),
	m_xsdGMonthDay(pKB, "http://www.w3.org/2001/XMLSchema#gMonthDay"),
	m_xsdGYear(pKB, "http://www.w3.org/2001/XMLSchema#gYear"),
	m_xsdGYearMonth(pKB, "http://www.w3.org/2001/XMLSchema#gYearMonth"),
	m_xsdHexBinary(pKB, "http://www.w3.org/2001/XMLSchema#hexBinary"),
	m_xsdInt(pKB, "http://www.w3.org/2001/XMLSchema#int"),
	m_xsdInteger(pKB, "http://www.w3.org/2001/XMLSchema#integer"),
	m_xsdLanguage(pKB, "http://www.w3.org/2001/XMLSchema#language"),
	m_xsdLong(pKB, "http://www.w3.org/2001/XMLSchema#long"),
	m_xsdName(pKB, "http://www.w3.org/2001/XMLSchema#Name"),
	m_xsdNCName(pKB, "http://www.w3.org/2001/XMLSchema#NCName"),
	m_xsdNegativeInteger(pKB, "http://www.w3.org/2001/XMLSchema#negativeInteger"),
	m_xsdNMTOKEN(pKB, "http://www.w3.org/2001/XMLSchema#NMTOKEN"),
	m_xsdNonNegativeInteger(pKB, "http://www.w3.org/2001/XMLSchema#nonNegativeInteger"),
	m_xsdNonPositiveInteger(pKB, "http://www.w3.org/2001/XMLSchema#nonPositiveInteger"),
	m_xsdNormalizedString(pKB, "http://www.w3.org/2001/XMLSchema#normalizedString"),
	m_xsdPositiveInteger(pKB, "http://www.w3.org/2001/XMLSchema#positiveInteger"),
	m_xsdShort(pKB, "http://www.w3.org/2001/XMLSchema#short"),
	m_xsdString(pKB, "http://www.w3.org/2001/XMLSchema#string"),
	m_xsdTime(pKB, "http://www.w3.org/2001/XMLSchema#time"),
	m_xsdToken(pKB, "http://www.w3.org/2001/XMLSchema#token"),
	m_xsdUnsignedByte(pKB, "http://www.w3.org/2001/XMLSchema#unsignedByte"),
	m_xsdUnsignedInt(pKB, "http://www.w3.org/2001/XMLSchema#unsignedInt"),
	m_xsdUnsignedLong(pKB, "http://www.w3.org/2001/XMLSchema#unsignedLong"),
	m_xsdUnsignedShort(pKB, "http://www.w3.org/2001/XMLSchema#unsignedShort"),
	m_xsdYearMonthDuration(pKB, "http://www.w3.org/2001/XMLSchema#yearMonthDuration")
{
}

pmnt::ResourceId pmnt::UriLib::translateReservedPredicate(ResourceId predicateId) const
{
	if (predicateId == m_parDirectSubClassOf.id())
	{
		return m_rdfsSubClassOf.id();
	}
	else if (predicateId == m_parDirectType.id())
	{
		return m_rdfType.id();
	}
	else
	{
		return predicateId;
	}
}
