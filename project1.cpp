/*
 * Contains the the apply function, the cofactors and quantification functions.
 *
 * For Project 1, implement
 * (1) apply, also handles probabilities
 * (2) negative_cofactor
 * (3) positive_cofactor, 
 * (4) boolean_difference
 * (5) sort by influence
 */

#include "project1.h"
int traverse_and_collect_var(bdd_ptr np,char *var, int number);

using namespace std;

// wrapper function to allow calling with the operation, i.e apply(bdd1, bdd2, "or")
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, string o)
{
  operation dop;
  if (!dop.set_operation(o))
  {
    return 0;
  }
  
  return apply(bdd1, bdd2, dop);
}

// apply implements an arbitrary operation (specified in op) on two BDDs
// bdd_tables is used to handle the book keeping (see bdd_tables.h).
//
// apply works recursively one the idea that given an arbitrary operation $, 
// and functions f and g: f $ g = a'(fa' $ ga') + a(fa $ ga), 
// where a is a variable, fa' is the negative cofactor etc.
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, operation &op)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  // ... your code goes here
  bdd_ptr result;
  bdd_ptr neg, pos;
  if (op.get_operation()=="and")
  {
    if ((bdd1->is_zero())||(bdd2->is_zero()))
      return bdd1->zero;
    if (bdd1->is_one())
      return bdd2;
    if (bdd2->is_one())
      return bdd1;
  }
  else if (op.get_operation()=="or")
  {
    if ((bdd1->is_one())||(bdd2->is_one()))
      return bdd1->one;
    if (bdd1->is_zero())
      return bdd2;
    if (bdd2->is_zero())
      return bdd1;
  }

  else if (op.get_operation()=="xor")
  {
    if ((bdd1->is_zero() && bdd2->is_one())||(bdd1->is_one() && bdd2->is_zero()))
      return bdd1->one;
    else if ((bdd1->is_zero() && bdd2->is_zero())||(bdd1->is_one() && bdd2->is_one()))
      return bdd1->zero;
    else if (bdd1->is_zero())
      return bdd2;
    else if (bdd2->is_zero())
      return bdd1;
    /*else if (bdd1->is_one())
    {
      neg = apply(bdd1, bdd2->neg_cf, op);
      pos = apply(bdd1, bdd2->pos_cf, op);
      bdd2->neg_cf = neg;
      bdd2->pos_cf = pos;
      bdd2->probability = 0.5*bdd1->neg_cf->probability + 0.5*bdd1->pos_cf->probability;
      return bdd2;
    }
    else if (bdd2->is_one())
    {
      neg = apply(bdd1->neg_cf, bdd2, op);
      pos = apply(bdd1->pos_cf, bdd2, op);
      bdd1->neg_cf = neg;
      bdd1->pos_cf = pos;
      bdd1->probability = 0.5*bdd1->neg_cf->probability + 0.5*bdd1->pos_cf->probability;
      return bdd1;
    }*/
  } 
  

    bdd_ptr compute_table_result_exist = tables.find_in_computed_table(op.get_operation(), bdd1, bdd2);
    
    if (compute_table_result_exist)
      return compute_table_result_exist;

    
    char next_var = find_next_var(bdd1, bdd2);
    if ((bdd1->var==next_var)&&(bdd2->var==next_var))
    {
      neg = apply(bdd1->neg_cf, bdd2->neg_cf, op);
      pos = apply(bdd1->pos_cf, bdd2->pos_cf, op);
      if(neg==pos)
      {
        return neg;
      }
      result = tables.find_in_unique_table(next_var,neg,pos);
      if (!result)
      {
        result = tables.create_and_add_to_unique_table(next_var, neg, pos);
      }
      result->probability = 0.5*result->neg_cf->probability + 0.5*result->pos_cf->probability;
      
      tables.insert_computed_table(op.get_operation(), bdd1, bdd2, result);
      return result;
    }

    else if(bdd1->var==next_var)
    {
      neg = apply(bdd1->neg_cf, bdd2, op);
      pos = apply(bdd1->pos_cf, bdd2, op);
      if(neg==pos)
      {
        return neg;
      }

      result = tables.find_in_unique_table(next_var,neg,pos);
      if (!result)
      {
        result = tables.create_and_add_to_unique_table(next_var, neg, pos);
      }

      result->probability = 0.5*result->neg_cf->probability + 0.5*result->pos_cf->probability;
      
      tables.insert_computed_table(op.get_operation(), bdd1, bdd2, result);//
      return result;
    }

    else if(bdd2->var==next_var)
    {
      neg = apply(bdd1, bdd2->neg_cf, op);
      pos = apply(bdd1, bdd2->pos_cf, op);
      if(pos==neg)
      {
        return neg;
      }
      result  = tables.find_in_unique_table(next_var,neg,pos);
      if (!result)
      {
        result = tables.create_and_add_to_unique_table(next_var, neg, pos);
      }

      result->probability = 0.5*result->neg_cf->probability + 0.5*result->pos_cf->probability;
      
      tables.insert_computed_table(op.get_operation(), bdd1, bdd2, result);//
      return result;
    }
  // change the return value when you're finished

}

// negative_cofactor takes the BDD pointed to by np, 
// and returns the negative cofactor with respect to var.
bdd_ptr negative_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  bdd_ptr new_pointer;
  bdd_ptr neg;
  bdd_ptr pos;
  
  if ((np->is_zero())||(np->is_one()))
  {    
    return np;
  }
  else
  {
    if(var!=np->var)
    {
      if(np->has_var(var)==0)
      {
        new_pointer = np;
      }
      else if(np->has_var(var)==1)
      {
        neg = negative_cofactor(np->neg_cf, var);
        pos = negative_cofactor(np->pos_cf, var);
        if(neg==pos)
        {
          return neg;
        }
        bdd_ptr find_result = tables.find_in_unique_table(np->var, neg, pos);
        if (find_result)
        {
          new_pointer = find_result;
        }
        else
        {
          new_pointer = tables.create_and_add_to_unique_table(np->var, neg, pos);
	  new_pointer->probability = 0.5 * new_pointer->neg_cf->probability + 0.5 * new_pointer->pos_cf->probability;
        }
      }
    }
    else if(var==np->var)
    {
      new_pointer = np->neg_cf;     
    }
  }

  return new_pointer;
  
  // once you add your code, remove this and return your own result
}

// posative_cofactor takes the BDD pointed to by np, 
// and returns the posative cofactor with respect to var.
bdd_ptr positive_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  bdd_ptr new_pointer;
  bdd_ptr neg;
  bdd_ptr pos;
  
  if ((np->is_zero())||(np->is_one()))
  {    
    return np;
  }
  else
  {
    if(var!=np->var)
    {
      if(np->has_var(var)==0)
      {
        new_pointer = np;
      }
      else if(np->has_var(var)==1)
      {
        neg = positive_cofactor(np->neg_cf, var);
        pos = positive_cofactor(np->pos_cf, var);
        if(neg==pos)
        {
          return neg;
        }
        bdd_ptr find_result = tables.find_in_unique_table(np->var, neg, pos);
        if (find_result)
        {

          new_pointer = find_result;
        }
        else
        {
          new_pointer = tables.create_and_add_to_unique_table(np->var, neg, pos);
          new_pointer->probability = 0.5 * new_pointer->neg_cf->probability + 0.5 * new_pointer->pos_cf->probability;
        }
      }
    }
    else if(var==np->var)
    {
        new_pointer = np->pos_cf;        
    }
  }

  return new_pointer;  

  // once you add your code, remove this and return your own result

}

// boolean_difference takes the BDD pointed to by np, 
// and returns the boolean difference with respect to var.
bdd_ptr boolean_difference(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  bdd_ptr neg_bdd;
  bdd_ptr pos_bdd;
  
  bdd_ptr difference_bdd;
  
  neg_bdd = negative_cofactor(np, var);
  pos_bdd = positive_cofactor(np, var);
  
  difference_bdd = apply(neg_bdd, pos_bdd, "xor");

  return difference_bdd; 

  // once you add your code, remove this and return your own result
  
}


// sort_by_influence calculates the influence of all the variables in np
// and displays them in descending order (most influent variable is
// shown first). For this task you can assume the number of variable
// is no greater than 20.
bdd_ptr sort_by_influence(bdd_ptr np)
{
  //... your code goes here
  char var[20];

  struct output
  {
    char variable;
    float prob;
  } output_result[20];

  bdd_ptr difference_bdd;

  int number_of_var = 0;
  number_of_var = traverse_and_collect_var(np,var,number_of_var);

  for (int i=0; i<number_of_var; i++)
  {
    difference_bdd = boolean_difference(np, var[i]);
    output_result[i].variable = var[i];
    output_result[i].prob = difference_bdd->probability;
  }

  for (int i=0; i<number_of_var; i++)
  {
    for (int j=i+1; j<number_of_var; j++)
    {
      if(output_result[j].prob>output_result[i].prob)
      {
        float temp_f = output_result[i].prob;
        char temp_c  = output_result[i].variable;

        output_result[i].variable = output_result[j].variable;
        output_result[i].prob     = output_result[j].prob;

        output_result[j].variable = temp_c;
        output_result[j].prob     = temp_f;
      }
    }
  }

  for (int i=0; i<number_of_var; i++)
  {
    cout<<output_result[i].variable<<","<<output_result[i].prob<<endl;
  }

  // this function does not alter the current node, so np must be
  // returned at the end
  return np; 
}

int traverse_and_collect_var(bdd_ptr np,char *var, int number)
{
  if (np->is_terminal())
  {
    return number;
  }
  else
  {
    number = traverse_and_collect_var(np->neg_cf,var,number);
    number = traverse_and_collect_var(np->pos_cf,var,number);
    for(int i=0; i < number; i++)
    {
      if(np->var == var[i])
        return number;   
    }
    *(var+number) = np->var;
    return number+1;
  }
}








