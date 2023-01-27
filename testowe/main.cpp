#include <iostream>

using namespace std;

//efficiently calculating a*b
long long log_mul(long long a, long long b){
    long long res = 0;
    long long res_accum = a;
    long long count = 0;
    long long acc_count = 1;
    while(acc_count*2 <= b){
        res_accum*=2;
        acc_count*=2;
    }
    res += res_accum;
    count += acc_count;
    while(b - count >= 1){
        while(count + acc_count >= b + 1){
            acc_count /= 2;
            res_accum /= 2;
        }
        res += res_accum;
        count += acc_count;
    }

    return res;
}

long long log_div(long long a, long long b){
    long long res = 0;
    long long remainder = a;
    long long temp = b;
    long long counter = 1;

    if(temp >= remainder + 1){
        return 0;
    }

    while(temp*2 <= remainder){
        temp *= 2;
        counter *= 2;
    }

    res += counter;
    remainder -= temp;

    while(remainder >= b) {
        while(remainder - temp + 1 <= 0){
            temp /= 2;
            counter /= 2;
        }
        remainder -= temp;
        res += counter;
    }
    // cout<<remainder<<endl;
    return res;
}

long long idiot_div(long long a, long long b){
    long long left = a;
    long long res = 0;
    while(left >= b){
        left-=b;
        res++;
    }
    cout<<"Mod: "<<left<<endl;
    return res;
}

void de(long long& m,long long& n,long long& x,long long& y,long long& z);

int main(int argc, char const *argv[])
{
    long long a,b;
    do{
        cin>>a>>b;
        cout<<"= "<<log_div(a,b)<<endl;
    } while( true );
    // long long m,n,x,y,nwd;
    // cin>>m>>n;
    // de(m,n,x,y,nwd);
    // cout<<"x: "<<x<<endl<<"y: "<<y<<endl<<"nwd: "<<nwd<<endl;

    return 0;
}

void de(long long& m,long long& n,long long& x,long long& y,long long& z){
    long long a,b,r,s,reszta,iloraz,rr,ss,tmp;
    a=m;
    b=n;
    x=1;
    y=0;
    r=n;
    s=m-1;
    while (b>0){
            reszta=a%b;
            iloraz=a/b;
            a=b;
            b=reszta;
            rr=r;
            tmp=iloraz*r;
            if (x<tmp){
                r=n*iloraz;
            } else {
                r=0;
            }
            r=r+x;
            r=r-tmp;
            ss=s;
            tmp=iloraz*s;
            if (y<tmp){
                s=m*iloraz;
            } else {
                s=0;
            }
            s=s+y;
            s=s-tmp;
            x=rr;
            y=ss;
    }
    z=a;
}

