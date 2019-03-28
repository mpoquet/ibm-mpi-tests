/****************************************************************************

 MESSAGE PASSING INTERFACE TEST CASE SUITE

 Copyright IBM Corp. 1995

 IBM Corp. hereby grants a non-exclusive license to use, copy, modify, and
 distribute this software for any purpose and without fee provided that the
 above copyright notice and the following paragraphs appear in all copies.

 IBM Corp. makes no representation that the test cases comprising this
 suite are correct or are an accurate representation of any standard.

 In no event shall IBM be liable to any party for direct, indirect, special
 incidental, or consequential damage arising out of the use of this software
 even if IBM Corp. has been advised of the possibility of such damage.

 IBM CORP. SPECIFICALLY DISCLAIMS ANY WARRANTIES INCLUDING, BUT NOT LIMITED
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS AND IBM
 CORP. HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

****************************************************************************

 These test cases reflect an interpretation of the MPI Standard.  They are
 are, in most cases, unit tests of specific MPI behaviors.  If a user of any
 test case from this set believes that the MPI Standard requires behavior
 different than that implied by the test case we would appreciate feedback.

 Comments may be sent to:
    Richard Treumann
    treumann@kgn.ibm.com

****************************************************************************
*/
#include "mpi.h"

main()
{
   int class,rc,me,tasks,i,*val,*keyval,flag,key,temp;
   MPI_Comm comm;

#define TAG_UB (1<<30)-1 

   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);
   MPI_Comm_size(MPI_COMM_WORLD,&tasks);

   MPI_Attr_get(MPI_COMM_WORLD,MPI_TAG_UB,&val,&flag);
   if(!flag) printf("ERROR in MPI_Attr_get: no val for MPI_TAG_UB\n");
   if(*val != TAG_UB)
      printf("ERROR in MPI_Attr_get: tag_ub = %d, should be %d\n",*val,TAG_UB);

   MPI_Attr_get(MPI_COMM_WORLD,MPI_HOST,&val,&flag);
   if(!flag) printf("ERROR in MPI_Attr_get: no val for MPI_HOST\n");
   if(*val != MPI_PROC_NULL)
      printf("ERROR in MPI_Attr_get: host = %d, should be %d\n",*val,MPI_PROC_NULL);

   MPI_Attr_get(MPI_COMM_WORLD,MPI_IO,&val,&flag);
   if(!flag) printf("ERROR in MPI_Attr_get: no val for MPI_IO\n");
   if(*val != MPI_ANY_SOURCE)
      printf("ERROR in MPI_Attr_get: io = %d, should be %d\n",*val,MPI_ANY_SOURCE);

   MPI_Keyval_create(MPI_NULL_COPY_FN,MPI_NULL_DELETE_FN,&key,0);

   MPI_Comm_dup(MPI_COMM_WORLD,&comm);

   MPI_Attr_get(comm,MPI_TAG_UB,&val,&flag);
   if(!flag) printf("ERROR in MPI_Attr_get: no val for MPI_TAG_UB\n");
   if(*val != TAG_UB)
      printf("ERROR in MPI_Attr_get: tag_ub = %d, should be %d\n",*val,TAG_UB);

   *val = 12345;
   MPI_Attr_put(comm,key,val);
   MPI_Attr_get(comm,key,&keyval,&flag);
   if(flag == 0)
      printf("ERROR in MPI_Attr_get: flag is false\n");
   if(*keyval != 12345)
      printf("ERROR in MPI_Attr_get: val = %d, should be %d\n",*keyval,12345);

   MPI_Errhandler_set(comm,MPI_ERRORS_RETURN);

   temp = key;
   MPI_Keyval_free(&key);
   if(key != MPI_KEYVAL_INVALID)
      printf("ERROR in MPI_Keyval_free: key not set to INVALID\n");

   key = temp;
   rc = MPI_Attr_get(comm,key,&keyval,&flag);
   MPI_Error_class(rc,&class);
   if(class != MPI_ERR_OTHER)
      printf("ERROR in MPI_Keyval_free: key not freed\n");

   rc = MPI_Attr_delete(comm,MPI_TAG_UB);
   if(rc == MPI_SUCCESS)
      printf("ERROR in MPI_Attr_delete, no error detected\n"); 

   rc = MPI_Attr_delete(comm,MPI_HOST);
   if(rc == MPI_SUCCESS)
      printf("ERROR in MPI_Attr_delete, no error detected\n"); 

   rc = MPI_Attr_delete(comm,MPI_IO);
   if(rc == MPI_SUCCESS)
      printf("ERROR in MPI_Attr_delete, no error detected\n"); 

   MPI_Barrier(comm);
   if(me == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
