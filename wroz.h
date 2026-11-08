double calcularEPG(double masa,double altura){
    return (9.8) * (masa * altura);
}

double calcularEC(double masa,double velocidad){
    return (0.5) * masa * velocidad * velocidad;
}

double calcularWroz(double VelocidadInicial,double VelocidadFinal,double AlturaInicial,double AlturaFinal,double MasaCoete){
    double Wroz, EMi, EMf, EPGi, ECi, ECf, EPGf;

    ECi = calcularEC(MasaCoete, VelocidadInicial);
    EPGi = calcularEPG(MasaCoete, AlturaInicial);
    EMi = ECi + EPGi;

    ECf = calcularEC(MasaCoete, VelocidadFinal);
    EPGf = calcularEPG(MasaCoete, AlturaFinal);
    EMf = ECf + EPGf;

    Wroz = EMf - EMi;
    return Wroz;
}