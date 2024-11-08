#include "KinematicsGA.h"
#include "openGA.hpp"
#include "../../utils.h"

/**
 * We use openGA Assist to generate code faster
**/
using std::string;

// KinematicsGA::KinematicsGA() {}

struct MySolution
{
    double var1;
    double var2;
    double var3;

    string to_string() const
    {
        return
            string("{")
            +  "var1:"+std::to_string(var1)
            +", var2:"+std::to_string(var2)
            +", var3:"+std::to_string(var3)
            +"}";
    }
};

struct MyMiddleCost
{
    // This is where the results of simulation
    // is stored but not yet finalized.
    double objective1;
};

typedef EA::Genetic<MySolution,MyMiddleCost> GA_Type;
typedef EA::GenerationType<MySolution,MyMiddleCost> Generation_Type;

void init_genes(MySolution& p,const std::function<double(void)> &rnd01)
{
    // rnd01() gives a random number in 0~1
    p.var1=0.0+10*rnd01();
    p.var2=0.0+10*rnd01();
    p.var3=0.0+10*rnd01();
}

bool eval_solution(
    const MySolution& p,
    MyMiddleCost &c)
{
    const double& var1=p.var1;
    const double& var2=p.var2;
    const double& var3=p.var3;

    c.objective1=2*var1 - 3*var2 + 4*var3;
    return true; // solution is accepted
}

MySolution mutate(
    const MySolution& X_base,
    const std::function<double(void)> &rnd01,
    double shrink_scale)
{
    MySolution X_new;
    const double mu = 0.2*shrink_scale; // mutation radius (adjustable)
    bool in_range;
    do{
        in_range=true;
        X_new=X_base;
        X_new.var1+=mu*(rnd01()-rnd01());
        in_range=in_range&&(X_new.var1>=0.0 && X_new.var1<10.0);
        X_new.var2+=mu*(rnd01()-rnd01());
        in_range=in_range&&(X_new.var2>=0.0 && X_new.var2<10.0);
        X_new.var3+=mu*(rnd01()-rnd01());
        in_range=in_range&&(X_new.var3>=0.0 && X_new.var3<10.0);
    } while(!in_range);
    return X_new;
}

MySolution crossover(
    const MySolution& X1,
    const MySolution& X2,
    const std::function<double(void)> &rnd01)
{
    MySolution X_new;
    double r;
    r=rnd01();
    X_new.var1=r*X1.var1+(1.0-r)*X2.var1;
    r=rnd01();
    X_new.var2=r*X1.var2+(1.0-r)*X2.var2;
    r=rnd01();
    X_new.var3=r*X1.var3+(1.0-r)*X2.var3;
    return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X)
{
    // finalize the cost
    double final_cost=0.0;
    final_cost+=X.middle_costs.objective1;
    return final_cost;
}


void SO_report_generation(
    int generation_number,
    const EA::GenerationType<MySolution,MyMiddleCost> &last_generation,
    const MySolution& best_genes)
{
    qDebug()
        <<"Generation ["<<generation_number<<"], "
        <<"Best="<<last_generation.best_total_cost<<", "
        <<"Average="<<last_generation.average_cost<<", "
        <<"Best genes=("<<best_genes.to_string()<<")"<<", "
        <<"Exe_time="<<last_generation.exe_time
        <<Qt::endl;
}

void KinematicsGA::proximalReset()
{
    proximal_act.reset();
}

void KinematicsGA::distalReset()
{
    distal_act.reset();
}

void KinematicsGA::calculate(Point3D& prox, Point3D& dist)
{

}

void KinematicsGA::testGA()
{
    EA::Chronometer timer;
    timer.tic();

    GA_Type ga_obj;
    ga_obj.problem_mode=EA::GA_MODE::SOGA;
    ga_obj.multi_threading=true;
    ga_obj.idle_delay_us = 1;
    ga_obj.verbose=false;
    ga_obj.population=200;
    ga_obj.generation_max=1000;
    ga_obj.calculate_SO_total_fitness=calculate_SO_total_fitness;
    ga_obj.init_genes=init_genes;
    ga_obj.eval_solution=eval_solution;
    ga_obj.mutate=mutate;
    ga_obj.crossover=crossover;
    ga_obj.SO_report_generation=SO_report_generation;
    ga_obj.crossover_fraction=0.7;
    ga_obj.mutation_rate=0.2;
    ga_obj.best_stall_max=10;
    ga_obj.elite_count=10;
    ga_obj.solve();

    qDebug() <<"The problem is optimized in "<<timer.toc()<<" seconds."<<Qt::endl;

}
