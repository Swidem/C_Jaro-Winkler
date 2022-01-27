#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define alfa 0.9 // our factor used in main at the end

//define used to jaro function
#define TRUE    1 
#define FALSE   0
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
 
double jaro(const char *str1, const char *str2) {
    // length of ints
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
 
    // jeśli obie zmienne są puste - zwróć 1
    // jeśli tylko 1 z nich to zwróć 0
    if (str1_len == 0) return str2_len == 0 ? 1.0 : 0.0;
 
    // max distance between ints 
    int match_distance = (int) max(str1_len, str2_len)/2 - 1;
 
    // tablica składająca się z bool która oznajmiają czy chary w zmiennych mają match
    int *str1_matches = calloc(str1_len, sizeof(int));
    int *str2_matches = calloc(str2_len, sizeof(int));
 
    // components of our algorithm - t and m
    double matches = 0.0;
    double transpositions = 0.0;
 
    // finding matches
    for (int i = 0; i < str1_len; i++) {
        
        int start = max(0, i - match_distance);
        int end = min(i + match_distance + 1, str2_len);
 
        for (int k = start; k < end; k++) {
            // if str2 already has match then continue
            if (str2_matches[k]) continue;
            // if str1 i str2 don't have match
            if (str1[i] != str2[k]) continue;
            // otherwise it's a match
            str1_matches[i] = TRUE;
            str2_matches[k] = TRUE;
            matches++;
            break;
        }
    }
 
    // if there's no matches return 0
    if (matches == 0) {
        free(str1_matches);
        free(str2_matches);
        return 0.0;
    }
 
    // calculating transpositions
    int k = 0;
    for (int i = 0; i < str1_len; i++) {
        // if there's no matches in str1 continue
        if (!str1_matches[i]) continue;
        // if there's no matches in str2 add to k
        while (!str2_matches[k]) k++;
        // add to transpositions
        if (str1[i] != str2[k]) transpositions++;
        k++;
    }
 
    // according to the way of counting we're dividing by 2
    transpositions /= 2.0;
 
    // freeing up space
    free(str1_matches);
    free(str2_matches);
 
    // calculating Jaro
    return ((matches / str1_len) +
        (matches / str2_len) +
        ((matches - transpositions) / matches)) / 3.0;
}


// Jaro Winkler distance
double jaro_winkler(const char *str1, const char *str2)
{
    double jaro_dist = jaro(str1, str2);
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
    
    // if Jaro is bigger than 0.7
    if (jaro_dist > 0.7) {
  
        // looking for the length of prefix
        int prefix = 0;
  
        for (int i = 0;
             i < min(str1_len, str2_len); i++) {
            // if words matching
            if (str1[i] == str2[i])
                prefix++;
            else
                break;
        }
  
        // max 4 characters allowed in prefix
        prefix = min(4, prefix);
  
        // calculating Jaro-Winkler distance
        jaro_dist += 0.1 * prefix * (1 - jaro_dist);
    }
  
    return jaro_dist;
}


#define NKEY 278	//number of elements in array 
#define LINE 256	//max number of characters in line

//stoplist
char*key[NKEY] = {"—", "a", "aby", "ach", "acz", "aczkolwiek", "aj", "albo", "ale", "ależ", "ani", "aż", "bardziej", "bardzo", "bo", "bowiem", "by", "byli", "bynajmniej", "być", "była", "było", "były", "będzie", "będą", "cali", "cała", "cały", "ci", "cię", "ciebie", "co", "cokolwiek", "coś", "czasami", "czasem", "czemu", "czy", "czyli", "daleko", "dla", "dlaczego", "dlatego", "do", "dobrze", "dokąd", "dość", "dużo", "dwa", "dwaj", "dwie", "dwoje", "dziś", "dzisiaj", "gdy", "gdyby", "gdyż", "gdzie", "gdziekolwiek", "gdzieś", "go", "i", "ich", "ile", "im", "inna", "inne", "inny", "innych", "iż", "ja", "ją", "jak", "jakaś", "jakby", "jaki", "jakichś", "jakie", "jakiś", "jakiż", "jakkolwiek", "jako", "jakoś", "je", "jeden", "jedna", "jedno", "jednak", "jednakże", "jego", "jej", "jemu", "jest", "jestem", "jeszcze", "jeśli", "jeżeli", "już", "ją", "każdy", "kiedy", "kilka", "kimś", "kto", "ktokolwiek", "ktoś", "która", "które", "którego", "której", "który", "których", "którym", "którzy", "ku", "lat", "lecz", "lub", "ma", "mają", "mało", "mam", "mi", "mimo", "między", "mną", "mnie", "mogą", "moi", "moim", "moja", "moje", "może", "możliwe", "można", "mój", "mu", "musi", "my", "na", "nad", "nam", "nami", "nas", "nasi", "nasz", "nasza", "nasze", "naszego", "naszych", "natomiast", "natychmiast", "nawet", "nią", "nic", "nich", "nie", "niech", "niego", "niej", "niemu", "nigdy", "nim", "nimi", "niż", "no", "o", "obok", "od", "około", "on", "ona", "one", "oni", "ono", "oraz", "oto", "owszem", "pan", "pana", "pani", "po", "pod", "podczas", "pomimo", "ponad", "ponieważ", "powinien", "powinna", "powinni", "powinno", "poza", "prawie", "przecież", "przed", "przede", "przedtem", "przez", "przy", "roku", "również", "sam", "sama", "są", "się", "skąd", "sobie", "sobą", "sposób", "swoje", "ta", "tak", "taka", "taki", "takie", "także", "tam", "te", "tego", "tej", "temu", "ten", "teraz", "też", "to", "tobą", "tobie", "toteż", "trzeba", "tu", "tutaj", "twoi", "twoim", "twoja", "twoje", "twym", "twój", "ty", "tych", "tylko", "tym", "u", "w", "wam", "wami", "was", "wasz", "wasza", "wasze", "we", "według", "wiele", "wielu", "więc", "więcej", "wszyscy", "wszystkich", "wszystkie", "wszystkim", "wszystko", "wtedy", "wy", "właśnie", "z", "za", "zapewne", "zawsze", "ze", "zł", "znowu", "znów", "został", "żaden", "żadna", "żadne", "żadnych", "że", "żeby"};

int main(){
    printf("\nExpected similarity (alpha) equals 0.9\n\n");

    FILE *wejscie = fopen("pt.txt", "r");
    FILE *wyjscie = fopen("pt_out.txt", "w");
    char *bufor = (char *)malloc(LINE * sizeof(char));
    char *pars;
    int k, czy;

    while ( fgets ( bufor, LINE, wejscie ) != NULL ) 
    {
        pars = strtok(bufor, " \n,\t.\r");  //exclude key words 
        while (pars != NULL) 
        {
            for(k=0;k<=strlen(pars);k++)
            { ///changing to lowercase
                if(pars[k]>=65&&pars[k]<=90)
                    pars[k]=pars[k]+32;
            }
            czy = 0;
            for(k=0;k<NKEY;k++) //compare words to the text from *key; if there's in array than exclude
                if(strcmp(pars, key[k])==0)
                    czy = 1;

            if(czy!=1) // put code in out file
            {                        
                fputs(pars, wyjscie);
                fputs("\n", wyjscie);
            }
            pars = strtok(NULL, " \n,\t.\r");   //pars string to the moment until pars is '\0'
        }
    }
    fclose(wejscie);
    fclose(wyjscie);
    
    //we have 2 files with words - one with correctly written words and corrections with modifications
    //parsing file good.txt into array
    FILE *in_file;
    in_file = fopen("good.txt", "r");
    fseek( in_file, 0, SEEK_END );
    long fsize = ftell( in_file );
    fseek( in_file, 0, SEEK_SET );

    char *buf = malloc( fsize + 1 );
    fread( buf, fsize, 1, in_file ); // put array into char
    fclose( in_file );

    char *items[50] = { NULL };
    char *pch;

    pch = strtok (buf," \t\n");
    int i = 0;
    while (pch != NULL)
    {
        items[i++] = pch;
        pch = strtok (NULL, " \t\n");
    }

    // 2 file similarly
    FILE *in_file2;
    in_file2 = fopen("corrections.txt", "r");
    fseek( in_file2, 0, SEEK_END );
    long fsize2 = ftell( in_file2 );
    fseek( in_file2, 0, SEEK_SET );

    char *buf2 = malloc( fsize2 + 1 );
    fread( buf2, fsize2, 1, in_file2 ); 
    fclose( in_file2 );

    char *items2[50] = { NULL };
    char *pch2;

    pch2 = strtok (buf2," \t\n");
    int j = 0;
    while (pch2 != NULL)
    {
        items2[j++] = pch2;
        pch2 = strtok (NULL, " \t\n");
    }

    //loop used to compare words from two txt files for our algorithm
    j=0;
    for( i = 0; i < 50; i++ )
    {
      if( items[i] != NULL )
        {
          //sprawdzamy czy próg podobieństwa będzie większy od zdefiniowanego alfa
          if(jaro_winkler(items[i], items2[j]) > alfa)
          {
            printf("%f %s - %s\n", jaro_winkler(items[i], items2[j]), items2[j], items[i]);
            j++;
          }
        }          
    }
    
    fclose(in_file);
    fclose(in_file2);

}