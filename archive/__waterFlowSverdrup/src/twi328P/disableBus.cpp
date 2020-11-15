//
//	twi328P::disableBus()
//
//	disables the twi bus
//
void twi328P::disableBus(void)
{
  // disable twi module, twi interrupt and acknowledgements
  TWCR &= ~(1 << (TWEN) | 1 << (TWIE) | 1 << (TWEA));
}
//	END twi328P::disableBus()
