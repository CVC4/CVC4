#include "cvc5_Term.h"

#include "api/cpp/cvc5.h"
#include "cvc5JavaApi.h"

using namespace cvc5::api;

/*
 * Class:     cvc5_Term
 * Method:    deletePointer
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_cvc5_Term_deletePointer(JNIEnv* env,
                                                    jclass,
                                                    jlong pointer)
{
  delete ((Term*)pointer);
}

/*
 * Class:     cvc5_Term
 * Method:    equals
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_equals(JNIEnv* env,
                                                 jobject,
                                                 jlong pointer1,
                                                 jlong pointer2)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* term1 = (Term*)pointer1;
  Term* term2 = (Term*)pointer2;
  // We compare the actual terms, not their pointers.
  return (jboolean)(*term1 == *term2);
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    compareTo
 * Signature: (JJ)I
 */
JNIEXPORT jint JNICALL Java_cvc5_Term_compareTo(JNIEnv* env,
                                                jobject,
                                                jlong pointer1,
                                                jlong pointer2)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* term1 = (Term*)pointer1;
  Term* term2 = (Term*)pointer2;
  if (*term1 < *term2)
  {
    return (jint)-1;
  }
  if (*term1 == *term2)
  {
    return 0;
  }
  return (jint)1;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    getNumChildren
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_cvc5_Term_getNumChildren(JNIEnv* env,
                                                     jobject,
                                                     jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jint)current->getNumChildren();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    getChild
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_getChild(JNIEnv* env,
                                                jobject,
                                                jlong pointer,
                                                jint index)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* ret = new Term((*current)[(size_t)index]);
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    getId
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_getId(JNIEnv* env,
                                             jobject,
                                             jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jlong)current->getId();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    getKind
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_cvc5_Term_getKind(JNIEnv* env,
                                              jobject,
                                              jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jint)current->getKind();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    getSort
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_getSort(JNIEnv* env,
                                               jobject,
                                               jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Sort* ret = new Sort(current->getSort());
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    substitute
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_substitute__JJJ(JNIEnv* env,
                                                       jobject,
                                                       jlong pointer,
                                                       jlong termPointer,
                                                       jlong replacementPointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* term = (Term*)termPointer;
  Term* replacement = (Term*)replacementPointer;
  Term* ret = new Term(current->substitute(*term, *replacement));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    substitute
 * Signature: (J[J[J)J
 */
JNIEXPORT jlong JNICALL
Java_cvc5_Term_substitute__J_3J_3J(JNIEnv* env,
                                   jobject,
                                   jlong pointer,
                                   jlongArray termPointers,
                                   jlongArray replacementPointers)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  jsize termsSize = env->GetArrayLength(termPointers);
  jsize replacementsSize = env->GetArrayLength(replacementPointers);
  jlong* termElements = env->GetLongArrayElements(termPointers, nullptr);
  jlong* replacementElements =
      env->GetLongArrayElements(replacementPointers, nullptr);

  std::vector<Term> terms(termsSize);
  std::vector<Term> replacements(replacementsSize);

  for (jsize i = 0; i < termsSize; i++)
  {
    Term* term = (Term*)termElements[i];
    terms[i] = *term;
  }
  env->ReleaseLongArrayElements(termPointers, termElements, 0);

  for (jsize i = 0; i < replacementsSize; i++)
  {
    Term* term = (Term*)replacementElements[i];
    replacements[i] = *term;
  }
  env->ReleaseLongArrayElements(replacementPointers, replacementElements, 0);

  Term* ret = new Term(current->substitute(terms, replacements));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    hasOp
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_hasOp(JNIEnv* env,
                                                jobject,
                                                jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->hasOp();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getOp
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_getOp(JNIEnv* env,
                                             jobject,
                                             jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Op* ret = new Op(current->getOp());
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    isNull
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_isNull(JNIEnv* env,
                                                 jobject,
                                                 jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->isNull();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getConstArrayBase
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_getConstArrayBase(JNIEnv* env,
                                                         jobject,
                                                         jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* ret = new Term(current->getConstArrayBase());
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    getConstSequenceElements
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL
Java_cvc5_Term_getConstSequenceElements(JNIEnv* env, jobject, jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  std::vector<Term> terms = current->getConstSequenceElements();
  std::vector<long> termPointers(terms.size());
  for (size_t i = 0; i < terms.size(); i++)
  {
    termPointers[i] = (long)new Term(terms[i]);
  }

  jlongArray ret = env->NewLongArray(terms.size());
  env->SetLongArrayRegion(ret, 0, terms.size(), termPointers.data());

  return ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, nullptr);
}

/*
 * Class:     cvc5_Term
 * Method:    notTerm
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_notTerm(JNIEnv* env,
                                               jobject,
                                               jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* ret = new Term(current->notTerm());
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    andTerm
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_andTerm(JNIEnv* env,
                                               jobject,
                                               jlong pointer,
                                               jlong termPointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* term = (Term*)termPointer;
  Term* ret = new Term(current->andTerm(*term));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    orTerm
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_orTerm(JNIEnv* env,
                                              jobject,
                                              jlong pointer,
                                              jlong termPointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* term = (Term*)termPointer;
  Term* ret = new Term(current->orTerm(*term));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    xorTerm
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_xorTerm(JNIEnv* env,
                                               jobject,
                                               jlong pointer,
                                               jlong termPointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* term = (Term*)termPointer;
  Term* ret = new Term(current->xorTerm(*term));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    eqTerm
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_eqTerm(JNIEnv* env,
                                              jobject,
                                              jlong pointer,
                                              jlong termPointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* term = (Term*)termPointer;
  Term* ret = new Term(current->eqTerm(*term));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    impTerm
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_impTerm(JNIEnv* env,
                                               jobject,
                                               jlong pointer,
                                               jlong termPointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* term = (Term*)termPointer;
  Term* ret = new Term(current->impTerm(*term));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    iteTerm
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_iteTerm(
    JNIEnv* env, jobject, jlong pointer, jlong thenPointer, jlong elsePointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term* thenTerm = (Term*)thenPointer;
  Term* elseTerm = (Term*)elsePointer;
  Term* ret = new Term(current->iteTerm(*thenTerm, *elseTerm));
  return (jlong)ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    toString
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cvc5_Term_toString(JNIEnv* env,
                                                  jobject,
                                                  jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return env->NewStringUTF(current->toString().c_str());
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, nullptr);
}

/*
 * Class:     cvc5_Term
 * Method:    isInt32
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_isInt32(JNIEnv* env,
                                                  jobject,
                                                  jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->isInt32();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getInt32
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_cvc5_Term_getInt32(JNIEnv* env,
                                               jobject,
                                               jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jint)current->getInt32();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    isUInt32
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_isUInt32(JNIEnv* env,
                                                   jobject,
                                                   jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->isUInt32();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getUInt32
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_cvc5_Term_getUInt32(JNIEnv* env,
                                                jobject,
                                                jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jint)current->getUInt32();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    isInt64
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_isInt64(JNIEnv* env,
                                                  jobject,
                                                  jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->isInt64();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getInt64
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_getInt64(JNIEnv* env,
                                                jobject,
                                                jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jlong)current->getInt64();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    isUInt64
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_isUInt64(JNIEnv* env,
                                                   jobject,
                                                   jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->isUInt64();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getUInt64
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_getUInt64(JNIEnv* env,
                                                 jobject,
                                                 jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jlong)current->getUInt64();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}

/*
 * Class:     cvc5_Term
 * Method:    isInteger
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_isInteger(JNIEnv* env,
                                                    jobject,
                                                    jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->isInteger();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getInteger
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cvc5_Term_getInteger(JNIEnv* env,
                                                    jobject,
                                                    jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return env->NewStringUTF(current->getInteger().c_str());
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, nullptr);
}

/*
 * Class:     cvc5_Term
 * Method:    isString
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_cvc5_Term_isString(JNIEnv* env,
                                                   jobject,
                                                   jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  return (jboolean)current->isString();
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, (jboolean) false);
}

/*
 * Class:     cvc5_Term
 * Method:    getString
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_cvc5_Term_getString(JNIEnv* env,
                                                   jobject,
                                                   jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  std::wstring termString = current->getString();

  size_t length = termString.length();
  jchar* unicode = new jchar[length];
  const wchar_t* s = termString.c_str();
  for (size_t i = 0; i < length; i++)
  {
    unicode[i] = s[i];
  }
  jstring ret = env->NewString(unicode, length);
  delete[] unicode;
  return ret;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, nullptr);
}

/*
 * Class:     cvc5_Term
 * Method:    iterator
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_cvc5_Term_iterator(JNIEnv* env,
                                                jobject,
                                                jlong pointer)
{
  CVC5_JAVA_API_TRY_CATCH_BEGIN;
  Term* current = (Term*)pointer;
  Term::const_iterator* retPointer = new Term::const_iterator(current->begin());
  return (jlong)retPointer;
  CVC5_JAVA_API_TRY_CATCH_END_RETURN(env, 0);
}
