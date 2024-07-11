use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::error::Error;
use tera::{Context, Tera, Value};

#[derive(Deserialize, Serialize)]
struct Unit {
    name: String,
    suffix: String,
}

#[derive(Deserialize, Serialize)]
struct Quantity {
    name: String,
    default_unit_type: String,
    units: Vec<Unit>,
}

#[derive(Deserialize, Serialize)]
struct Items {
    quantities: Vec<Quantity>,
}

// Custom filter to capitalize the first letter of a string
fn capitalize_filter(value: &Value, _: &HashMap<String, Value>) -> tera::Result<Value> {
    // Ensure the value is a string
    let s = value
        .as_str()
        .ok_or_else(|| tera::Error::msg("Expected a string for capitalize_filter"))?;

    // Capitalize the first letter
    let mut c = s.chars();
    let capitalized = match c.next() {
        None => String::new(),
        Some(first) => first.to_uppercase().collect::<String>() + c.as_str(),
    };

    // Return the modified value
    Ok(Value::String(capitalized))
}

fn parse_and_feed(template_file: &str, params_file: &str) -> Result<String, Box<dyn Error>> {
    let template = std::fs::read_to_string(template_file)?;
    let params_str = std::fs::read_to_string(params_file)?;
    let items: Items = serde_json::from_str(&params_str)?;

    let context = Context::from_serialize(&items)?;

    let mut tera = Tera::new("res/**/*")?;

    tera.register_filter("capitalize", capitalize_filter);

    let gen = tera.render_str(&template, &context)?;

    Ok(gen)
}

fn main() {
    std::fs::create_dir_all("out/").unwrap();

    match parse_and_feed("res/quantity.tera", "res/quantity-params.json") {
        Ok(gen) => std::fs::write("out/quantity.h", &gen).unwrap(),
        Err(e) => println!("Error while parsing quantity: {}", e),
    }
}
