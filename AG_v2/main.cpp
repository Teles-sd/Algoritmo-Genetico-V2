#include<iostream>
#include<ctime>
#include<cstdlib>
#include<math.h>
using namespace std;

/*
 *      FUNCÕES
 */

int n_rand(int vMin, int vMax, bool prt = 0) {
    int max = vMax;
    if (vMin == 0) {
        max++;
    }
    if (vMin > 1) {
        max--;
    }
    int num = vMin + (rand() % max);
    if (prt)
        cout << "Random between " << vMin << " and " << vMax << " : " << num << endl;
    //num=1;        //teste
    return num;
}

/*
 *      CLASSES
 */

//BUILD DE "individuo" COM:
//o numero de bits(máx 25)
//valor minimo (-100) e máximo (100) do domínio

class individuo {
public:

    individuo() {
        for (int i = 0; i < 25; i++) {
            x_bin[i] = 0;
            y_bin[i] = 0;
        }
    }

    void build(int num_de_bits, int min, int max) {
        if (non_built) {
            non_built = 0;
            n = num_de_bits;
            V_min = min;
            V_max = max;
            x_dec = 0;
            y_dec = 0;
            f = 0;
            //define o primeiro indiv
            for (int i = 0; i < n; i++) {
                x_bin[i] = n_rand(0, 1);
                y_bin[i] = n_rand(0, 1);
            }
            update();
        } else {
            throw "Already built";
        }
    }

    void rebuild() {
        x_dec = 0;
        y_dec = 0;
        f = 0;
        //define o primeiro indiv
        for (int i = 0; i < n; i++) {
            x_bin[i] = n_rand(0, 1);
            y_bin[i] = n_rand(0, 1);
        }
        update();
    }

    //atualiza o valor da aptdão

    void update() {
        bin_dec();
        f = func(x_dec, y_dec);
    }

    //----------------------------------------------
    //----------------------------------------------
    //  GETs E SETs

    float get_x() {
        return x_dec;
    }

    float get_y() {
        return y_dec;
    }

    float get_f() {
        return f;
    }

    bool get_x_bin(int indice) {
        return x_bin[indice];
    }

    bool get_y_bin(int indice) {
        return y_bin[indice];
    }

    void set_x_bin(int indice, bool valor) {
        x_bin[indice] = valor;
    }

    void set_y_bin(int indice, bool valor) {
        y_bin[indice] = valor;
    }
    //----------------------------------------------
    //----------------------------------------------

    void print_info() {
        cout << "\n-------------------------------------------\n";
        cout << "X decimal: " << x_dec << "\nX binario:";
        for (int i = 0; i < n; i++) {
            cout << " " << x_bin[i];
        }
        cout << "\n\nY decimal: " << y_dec << "\nY binario:";
        for (int i = 0; i < n; i++) {

            cout << " " << y_bin[i];
        }
        cout << "\n\n -- Aptid: " << f << endl;
        cout << "\n-------------------------------------------\n";
    }

private:
    bool non_built = 1;
    int n;
    int V_min;
    int V_max;
    bool x_bin[25];
    bool y_bin[25];
    float x_dec;
    float y_dec;
    float f;

    void bin_dec() {
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //conversão pra inteiro do X
        float vaolr_inteiro = 0;
        for (int i = (n - 1); i >= 0; i--) {
            if (x_bin[i]) {
                vaolr_inteiro += pow(2, i);
            }
        }
        //conversão pra decimal do X
        float valor_decimal = ((vaolr_inteiro * 1.0 * (V_max - V_min)) / (pow(2, n) - 1)) + (V_min);
        x_dec = valor_decimal;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //conversão pra inteiro do Y
        vaolr_inteiro = 0;
        for (int i = (n - 1); i >= 0; i--) {
            if (y_bin[i]) {
                vaolr_inteiro += pow(2, i);
            }
        }
        //conversão pra decimal do Y
        valor_decimal = (vaolr_inteiro * (V_max - V_min)) / (pow(2, n) - 1) + (V_min);
        y_dec = valor_decimal;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }

    //função problema

    float func(float x, float y) {
        float aux;
        aux = 0.5 - ((pow(sin(sqrt(pow(x, 2) + pow(y, 2))), 2)) - 0.5) /
                pow(1 + (0.001 * (pow(x, 2) + pow(y, 2))), 2);

        return aux;
    }
};


//BUILD DE "população" COM:
//o numero de bits(máx 25)
//valor minimo (-100) e máximo (100) do domínio
//numero de indiviudos (máx 50 e sempre par)
//numero de gerações (max 200)

class population {
public:

    void build(int num_de_bits, int min, int max, int num_indv, int num_ger) {
        if (non_built) {
            n_bits = num_de_bits;
            n_ind = num_indv;
            n_ger = num_ger;
            for (int i = 0; i < n_ind; i++) {
                indv[i].build(num_de_bits, min, max);
            }
        } else {
            throw "Already built";
        }
    }

    void rebuild() {
        for (int i = 0; i < n_ind; i++) {
            indv[i].rebuild();
        }
    }

    //privilegia os indivíduos mais aptos
    //e cria os valores do melhor, media e desvio padrão cada geração

    void selection() {
        //zera os valores de aptidão e as parcelas de participação
        for (int i = 0; i < n_ind; i++) {
            val_apt[i] = 0;
            parc_part[i] = 0;
        }
        //zera as aptidão total e participação total
        apt_total = 0;
        part_total = 0;
        //define os valores de aptidão e a aptidão total
        for (int i = 0; i < n_ind; i++) {
            val_apt[i] = indv[i].get_f();
            apt_total += indv[i].get_f();
        }
        //define as parcelas de participação
        for (int i = 0; i < n_ind; i++) {
            parc_part[i] = (val_apt[i] / apt_total * 100000);
        }
        //define a participação total (somatória ou espaço amostral)
        for (int i = 0; i < n_ind; i++) {
            part_total += parc_part[i];
        }
        //----------------------------------------------------------
        int aux;
        int aptos[n_ind];
        for (int n = 0; n < n_ind; n++) {
            aptos[n] = n;
        }
        for (int n = 0; n < (n_ind - 1); n++) {
            for (int m = n + 1; m < n_ind; m++) {
                if (parc_part[aptos[n]] < parc_part[aptos[m]]) {
                    aux = aptos[n];
                    aptos[n] = aptos[m];
                    aptos[m] = aux;
                }
            }
        }
        melhor_de_cada[ger] = val_apt[aptos[0]];
        //----------------------------------------------------------
        media_de_cada[ger] = (apt_total / n_ind);
        //----------------------------------------------------------
        float auxl = 0;
        for (int i = 0; i < n_ind; i++) {
            auxl += pow((val_apt[i] - media_de_cada[ger]), 2);
        }
        dp_de_cada[ger] = sqrt(auxl / (n_ind - 1));
    }

    void reproduction() {

        //criando a roleta
        int qt = part_total;
        int roleta[qt];
        for (int x = 0; x < n_ind; x++) {
            for (int y = 0; y < parc_part[x]; y++) {
                qt--;
                roleta[qt] = x;
            }
        }

        /*
        //print da roleta
        cout << "||--------------------------||\n  Roleta:\n" << endl;
        int l = 0;
        for (int i = 0; i < part_total; i++) {
            if (l == 0) {
                cout << "\n ";
                l = 20;
            }
            cout << " " << roleta[i];
            l--;
        }
        //*/

        //crusamento
        bool new_indv[n_ind][2][n_bits]; // [indv][x/y][bit]

        for (int y = 0; y < (n_ind / 2); y++) {
            int par1;
            int par2;
            do {
                do {
                    par1 = n_rand(0, (part_total - 1));
                    par2 = n_rand(0, (part_total - 1));
                } while (roleta[par1] == roleta[par2]);
            } while (n_rand(1, 10) <= 8);

            short corte = n_rand(1, (n_bits - 1));

            for (int n = 0; n < n_bits; n++) {
                if (n < corte) {
                    new_indv[y][0][n] = indv[roleta[par1]].get_x_bin(n);
                    new_indv[y][1][n] = indv[roleta[par1]].get_y_bin(n);
                    //
                    new_indv[y + (n_ind / 2)][0][n] = indv[roleta[par2]].get_x_bin(n);
                    new_indv[y + (n_ind / 2)][1][n] = indv[roleta[par2]].get_y_bin(n);
                } else {
                    new_indv[y][0][n] = indv[roleta[par2]].get_x_bin(n);
                    new_indv[y][1][n] = indv[roleta[par2]].get_y_bin(n);
                    //
                    new_indv[y + (n_ind / 2)][0][n] = indv[roleta[par1]].get_x_bin(n);
                    new_indv[y + (n_ind / 2)][1][n] = indv[roleta[par1]].get_y_bin(n);
                }
            }
        }
        //salvando e atualizando nova geração
        for (int y = 0; y < n_ind; y++) {
            for (int n = 0; n < n_bits; n++) {
                indv[y].set_x_bin(n, new_indv[y][0][n]);
                indv[y].set_y_bin(n, new_indv[y][1][n]);
            }
            indv[y].update();
        }

        //mutação
        for (int y = 0; y < n_ind; y++) {
            for (int n = 0; n < n_bits; n++) {
                if (n_rand(1, 100) == 1) {
                    if (indv[y].get_x_bin(n)) {
                        indv[y].set_x_bin(n, 0);
                    } else {
                        indv[y].set_x_bin(n, 1);
                    }
                }
                if (n_rand(1, 100) == 1) {
                    if (indv[y].get_y_bin(n)) {
                        indv[y].set_y_bin(n, 0);
                    } else {
                        indv[y].set_y_bin(n, 1);
                    }
                }
            }
            indv[y].update();
        }

        ger++;
    }

    void print_info() {
        int aux;
        int aptos[n_ind];
        for (int n = 0; n < n_ind; n++) {
            aptos[n] = n;
        }
        for (int n = 0; n < (n_ind - 1); n++) {
            for (int m = n + 1; m < n_ind; m++) {
                if (parc_part[aptos[n]] < parc_part[aptos[m]]) {
                    aux = aptos[n];
                    aptos[n] = aptos[m];
                    aptos[m] = aux;
                }
            }
        }
        cout << "||--------------------------||" << endl;
        cout << "  GENERATION: " << ger << endl;
        cout << "  Fitness:     Participation:\n  ";
        for (int n = 0; n < n_ind; n++) {

            cout << val_apt[aptos[n]] << "  ||  " << parc_part[aptos[n]] << "\n  ";
        }
        cout << "\n  Total:\n  " << apt_total << "  || " << part_total << "\n\n";
    }

    float get_melhor_de_cada(int ger) {
        return melhor_de_cada[ger];
    }

    float get_media_de_cada(int ger) {
        return media_de_cada[ger];
    }

    float get_dp_de_cada(int ger) {
        return dp_de_cada[ger];
    }

    bool get_indv(int indiv, int x0_y1_f2, int indice) {
        if (x0_y1_f2 == 1) {
            return indv[indiv].get_y_bin(indice);
        } else if (x0_y1_f2 == 0) {
            return indv[indiv].get_x_bin(indice);
        } else if (x0_y1_f2 == 2) {
            return indv[indiv].get_f();
        } else {
            throw "\nno indice on \"get_indv\" \n";
        }
    }

    void set_indv(int indiv, bool x0_y1, int indice, int valor) {
        if (x0_y1) {
            indv[indiv].set_y_bin(indice, valor);
        } else {
            indv[indiv].set_x_bin(indice, valor);
        }
    }

    void reset_ger() {
        ger = 0;
    }


private:
    bool non_built = 1;
    //
    individuo indv[50];
    //
    int n_bits;
    int n_ind;
    int n_ger;
    //
    float val_apt[50];
    float apt_total;
    int parc_part[50];
    int part_total;
    //
    int ger = 0;
    float melhor_de_cada[201];
    float media_de_cada[201];
    float dp_de_cada[201];
    //
};

//CONSTRUTOR DE "ensaio" COM:
//o numero de bits(máx 25)
//valor minimo (-100) e máximo (100) do domínio
//numero de indiviudos (máx 50 e sempre par)
//numero de gerações (max 200)

class ensaio {
public:

    ensaio(int num_de_bits, int min, int max, int num_indv, int num_ger) {
        n_bits = num_de_bits;
        n_ind = num_indv;
        n_ger = num_ger;
        //
        br.build(num_de_bits, min, max, num_indv, num_ger);
        br.selection();
    }

    void gerar_cond_inic() {
        for (int i = 0; i < n_ind; i++) {
            for (int cr = 0; cr < 2; cr++) {
                for (int b = 0; b < n_bits; b++) {
                    cond_inic[i][cr][b] = br.get_indv(i, cr, b);
                }
            }
        }
    }

    void setar_cond_inic() {
        for (int i = 0; i < n_ind; i++) {
            for (int cr = 0; cr < 2; cr++) {
                for (int b = 0; b < n_bits; b++) {
                    br.set_indv(i, cr, b, cond_inic[i][cr][b]);
                }
            }
        }
    }

    void rodar_ger(int num_ger, bool prt = 0) {
        melhor_de_cada[0] = br.get_melhor_de_cada(0);
        media_de_cada[0] = br.get_media_de_cada(0);
        dp_de_cada[0] = br.get_dp_de_cada(0);
        //
        br.reset_ger();
        for (int i = 0; i < num_ger; i++) {
            br.reproduction();
            br.selection();
            if (prt) {
                br.print_info();
            }
            melhor_de_cada[i + 1] = br.get_melhor_de_cada(i + 1);
            if (melhor_de_cada[i + 1] > melhor_de_cada[i]) {
                n_melhor = i + 1;
            }
            media_de_cada[i + 1] = br.get_media_de_cada(i + 1);
            dp_de_cada[i + 1] = br.get_dp_de_cada(i + 1);
        }
        if (prt) {
            cout << "||-------------------------------------------||" << endl;
            cout << "        INFO: " << endl;
            cout << "        Melhor:      Media:       Desv.Padr.:\n  ";
            for (int n = 0; n <= num_ger; n++) {
                if (n < 10) {
                    cout << "G" << n << ":   ";
                } else if (n < 100) {
                    cout << "G" << n << ":  ";
                } else {
                    cout << "G" << n << ": ";
                }
                cout << melhor_de_cada[n] << "  ||  " << media_de_cada[n] << "  ||  " << dp_de_cada[n] << "\n  ";
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            cout << "\n        GERAL:" << endl;
            cout << "  G0:   " << melhor_de_cada[0] << "  ||  " << media_de_cada[0] << "  ||  " << dp_de_cada[0] << "\n\n";
            cout << "        Melhor:      Media:       Desv.Padr.:\n  ";
            if (n_melhor < 10) {
                cout << "G" << n_melhor << ":   ";
            } else if (n_melhor < 100) {
                cout << "G" << n_melhor << ":  ";
            } else {
                cout << "G" << n_melhor << ": ";
            }
            cout << melhor_de_cada[n_melhor] << "  ||  " << media_de_cada[n_melhor] << "  ||  " << dp_de_cada[n_melhor] << "\n  ";
        }
    }

    void rodar_ens(int num_ens, int num_cond_inc = 0, bool prt = 0) {
        //zerando
        for (int g = 0; g <= n_ger; g++) {
            m_melhor_final[g] = 0;
            m_media_final[g] = 0;
            m_dp_final[g] = 0;
        }
        //
        for (int c = 0; c < num_cond_inc; c++) {
            gerar_cond_inic();

            for (int e = 0; e < num_ens; e++) {
                rodar_ger(n_ger);
                setar_cond_inic();

                for (int g = 0; g <= n_ger; g++) {
                    m_melhor_final[g] += melhor_de_cada[g];
                    m_media_final[g] += media_de_cada[g];
                    m_dp_final[g] += dp_de_cada[g];

                }
            }
            br.rebuild();
            br.selection();
        }
        for (int g = 0; g <= n_ger; g++) {
            m_melhor_final[g] /= (num_cond_inc*num_ens);
            m_media_final[g] /= (num_cond_inc*num_ens);
            m_dp_final[g] /= (num_cond_inc*num_ens);
        }

        for (int g = 0; g < n_ger; g++) {
            if (m_melhor_final[g + 1] > m_melhor_final[g]) {
                n_melhor = g + 1;
            }
        }

        if (prt) {
            cout << "||-------------------------------------------||" << endl;
            cout << "        INFO: " << endl;
            cout << "        Melhor:      Media:       Desv.Padr.:\n  ";
            for (int n = 0; n <= n_ger; n++) {
                if (n < 10) {
                    cout << "G" << n << ":   ";
                } else if (n < 100) {
                    cout << "G" << n << ":  ";
                } else {
                    cout << "G" << n << ": ";
                }
                cout << m_melhor_final[n] << "  ||  " << m_media_final[n] << "  ||  " << m_dp_final[n] << "\n  ";
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            cout << "\n        GERAL:" << endl;
            cout << "  G0:   " << m_melhor_final[0] << "  ||  " << m_media_final[0] << "  ||  " << m_dp_final[0] << "\n\n";
            cout << "        Melhor:      Media:       Desv.Padr.:\n  ";
            if (n_melhor < 10) {
                cout << "G" << n_melhor << ":   ";
            } else if (n_melhor < 100) {
                cout << "G" << n_melhor << ":  ";
            } else {
                cout << "G" << n_melhor << ": ";
            }
            cout << m_melhor_final[n_melhor] << "  ||  " << m_media_final[n_melhor] << "  ||  " << m_dp_final[n_melhor] << "\n  ";
        }

    }


private:
    //
    population br;
    //
    int n_bits;
    int n_ind;
    int n_ger;
    //
    int n_melhor = 0;
    float melhor_de_cada[201];
    float media_de_cada[201];
    float dp_de_cada[201];
    //
    float m_melhor_final[201];
    float m_media_final[201];
    float m_dp_final[201];
    //
    bool cond_inic[50][2][25]; // [indv][x/y][bit]
};

/*
 *      MAIN
 */

int main() {
    //  INICIALIZAÇÕES:
    srand(time(0)); //seed de random de acordo com o tempo
    //*
    cout.precision(5); //precisão dos foats ajustada
    std::cout.setf(std::ios::fixed, std::ios::floatfield); // para 5 casas decimais
    //*/

    /*
    individuo a1;
    a1.build(25, -20, 20);
    a1.print_info();
    // */

    /*
    population koreanos;
    koreanos.build(13, -3, 3, 10, 20);
    koreanos.selection();
    koreanos.print_info();
    koreanos.reproduction();
    //*/

    /*
    population koreanos;
    koreanos.build(25, -100, 100, 50, 200);
    koreanos.selection();
    koreanos.print_info();
    for (int i = 0; i < 200; i++) {
        koreanos.reproduction();
        koreanos.selection();
        koreanos.print_info();
    }
    //*/

    /*
    ensaio final(25, -100, 100, 50, 200);
    final.rodar_ger(200, 1);
    //*/

    //*
    ensaio final(25, -100, 100, 50, 200);
    final.rodar_ens(5, 10, 1);
    //*/
    return 0;
}